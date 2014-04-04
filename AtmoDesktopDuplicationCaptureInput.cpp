
#include "stdafx.h"
#include "AtmoDesktopDuplicationCaptureInput.h"

#pragma comment( lib, "dxgi" )
#pragma comment( lib, "d3d11" )

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

static inline int divRoundClosest(const UINT n, const UINT d)
{
    return ((n + d / 2) / d);
}

CAtmoDesktopDuplicationCaptureInput::CAtmoDesktopDuplicationCaptureInput(CAtmoDynData *pAtmoDynData)
 : CAtmoInput(pAtmoDynData)
 , m_pOutput(NULL)
 , m_pOutputDuplication(NULL)
 , m_pD3D11Device(NULL)
 , m_pD3D11DeviceContext(NULL)
 , m_pTexture(NULL)
 , m_bThreadStarted(false)
{
}

CAtmoDesktopDuplicationCaptureInput::~CAtmoDesktopDuplicationCaptureInput(void)
{
    SafeRelease(&m_pOutputDuplication);
    SafeRelease(&m_pOutput);
    SafeRelease(&m_pTexture);
    SafeRelease(&m_pD3D11Device);
    SafeRelease(&m_pD3D11DeviceContext);
}

ATMO_BOOL CAtmoDesktopDuplicationCaptureInput::Open()
{
    HRESULT hr = S_OK;
    CAtmoConfig *pAtmoConfig = m_pAtmoDynData->getAtmoConfig();
    CAtmoDisplays *pAtmoDisplays = m_pAtmoDynData->getAtmoDisplays();

    // clear temp hsv image
    memset(&HSV_Img, 0, IMAGE_SIZE * sizeof(tHSVColor));

    int displayNr = pAtmoConfig->getLiveView_DisplayNr();
    if (displayNr >= pAtmoDisplays->getCount())
        displayNr = 0; // drop back to display 0 ;-)
    TAtmoDisplayInfo displayInfo = pAtmoDisplays->getDisplayInfo(displayNr);

    IDXGIAdapter1 *pAdapter = NULL;
    GetOutput(displayInfo.vert_ScreenPos, displayInfo.horz_ScreenPos, &m_pOutput, &pAdapter);
    if (m_pOutput == NULL) {
        goto done;
    }
    
    DXGI_OUTPUT_DESC outputDesc;
    m_pOutput->GetDesc(&outputDesc);

    m_lWidth = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
    m_lHeight = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;

    ID3D11DeviceContext *pContext = NULL;
    D3D_FEATURE_LEVEL Level;
    hr = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_pD3D11Device, &Level, &m_pD3D11DeviceContext);
    if (FAILED(hr)) {
        goto done;
    }

    D3D11_TEXTURE2D_DESC TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(TextureDesc));
    TextureDesc.ArraySize = 1;
    TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    TextureDesc.Width = m_lWidth;
    TextureDesc.Height = m_lHeight;
    TextureDesc.MipLevels = 1;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.Usage = D3D11_USAGE_STAGING;    

    hr = m_pD3D11Device->CreateTexture2D(&TextureDesc, NULL, &m_pTexture);
    if (FAILED(hr)) {
        goto done;
    }

    hr = m_pOutput->DuplicateOutput(m_pD3D11Device, &m_pOutputDuplication);
    if (FAILED(hr)) {
        goto done;
    }
    
    Run();
    m_bThreadStarted = true;
done:
    SafeRelease(&pAdapter);
    return true;
}

ATMO_BOOL CAtmoDesktopDuplicationCaptureInput::Close()
{
    if (m_bThreadStarted)
        Terminate();
    SafeRelease(&m_pOutputDuplication);
    SafeRelease(&m_pOutput);

    SafeRelease(&m_pTexture);
    SafeRelease(&m_pD3D11Device);
    SafeRelease(&m_pD3D11DeviceContext);
    return true;
}

void CAtmoDesktopDuplicationCaptureInput::ReadImage(BYTE *pData, UINT Stride)
{
    tRGBColor pixelColor;
    int index = 0;
    // TODO: Do Bilinear scaling to the CAP_HEIGHT/CAP_WIDTH size instead of point-scaling
    // Note: All the - 1 below can then go, as they are kludges to get to the border of the screen, and not a few pixels before
    // and line/col then define the start of the area of the image which should be averaged
    for(UINT y = 0; y < CAP_HEIGHT; y++)
    {
        UINT line = divRoundClosest((m_lHeight - 1) * y, CAP_HEIGHT - 1);
        BYTE *pLine = pData + line * Stride;
        for (UINT x = 0; x < CAP_WIDTH; x++)
        {
            UINT col = divRoundClosest((m_lWidth - 1) * x, CAP_WIDTH - 1);
            pixelColor.b = *(pLine + col * 4 + 0);
            pixelColor.g = *(pLine + col * 4 + 1);
            pixelColor.r = *(pLine + col * 4 + 2);
            HSV_Img[index++] = RGB2HSV(pixelColor);
        }
    }

    m_pAtmoDynData->getLivePacketQueue()->AddPacket( m_pAtmoColorCalculator->AnalyzeHSV( HSV_Img ) );
}

DWORD CAtmoDesktopDuplicationCaptureInput::Execute(void)
{
    HRESULT hr = S_OK;
    DXGI_OUTDUPL_FRAME_INFO FrameInfo;
    IDXGIResource *pDesktopResource = NULL;
    while (m_bTerminated == ATMO_FALSE)
    {
        hr = m_pOutputDuplication->AcquireNextFrame(1000, &FrameInfo, &pDesktopResource);
        if (SUCCEEDED(hr)) {
            if (FrameInfo.AccumulatedFrames > 0) {
                ID3D11Texture2D *pDesktopResource2D = NULL;
                hr = pDesktopResource->QueryInterface(&pDesktopResource2D);
                if (SUCCEEDED(hr)) {
                    m_pD3D11DeviceContext->CopyResource(m_pTexture, pDesktopResource2D);

                    D3D11_MAPPED_SUBRESOURCE MappedResource;
                    hr = m_pD3D11DeviceContext->Map(m_pTexture, 0, D3D11_MAP_READ, 0, &MappedResource);
                    if (SUCCEEDED(hr)) {
                        ReadImage((BYTE *)MappedResource.pData, MappedResource.RowPitch);
                        m_pD3D11DeviceContext->Unmap(m_pTexture, 0);
                    }
                    SafeRelease(&pDesktopResource2D);
                }
            }

            SafeRelease(&pDesktopResource);
            m_pOutputDuplication->ReleaseFrame();
        } else {
            if (hr == DXGI_ERROR_ACCESS_LOST) {
                SafeRelease(&m_pOutputDuplication);
                hr = m_pOutput->DuplicateOutput(m_pD3D11Device, &m_pOutputDuplication);
                if (FAILED(hr)) {
                    return 1;
                }
            } else if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
                // Moo!
            } else {
                // Error?
            }
        }
    }

    return 0;
}

HRESULT CAtmoDesktopDuplicationCaptureInput::GetOutput(LONG vertPos, LONG horzPost, IDXGIOutput1 **ppOutput1, IDXGIAdapter1 **ppAdapter1)
{
    HRESULT hr = S_OK;
    IDXGIFactory1 * pFactory = NULL;
    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory));
    if (FAILED(hr)) {
        return false;
    }

    UINT i = 0;
    IDXGIAdapter1 * pAdapter;
    while (pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND && !(*ppOutput1))
    {
        UINT j = 0;
        IDXGIOutput * pOutput = NULL;
        while (pAdapter->EnumOutputs(j, &pOutput) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_OUTPUT_DESC desc;
            if (SUCCEEDED(pOutput->GetDesc(&desc)) && desc.AttachedToDesktop) {
                if (desc.DesktopCoordinates.top == vertPos && desc.DesktopCoordinates.left == horzPost) {
                    if (SUCCEEDED(pOutput->QueryInterface(ppOutput1))) {
                        *ppAdapter1 = pAdapter;
                        (*ppAdapter1)->AddRef();
                    }
                    break;
                }
            }
            SafeRelease(&pOutput);
            j++;
        }
        SafeRelease(&pAdapter);
        i++;
    }

    if (*ppOutput1 == NULL) {
        pFactory->EnumAdapters1(0, &pAdapter);
        if (pAdapter) {
            IDXGIOutput * pOutput = NULL;
            pAdapter->EnumOutputs(0, &pOutput);
            if (pOutput) {
                if (SUCCEEDED(pOutput->QueryInterface(ppOutput1))) {
                    *ppAdapter1 = pAdapter;
                    (*ppAdapter1)->AddRef();
                }
            }
            SafeRelease(&pOutput);
        }
        SafeRelease(&pAdapter);
    }

    pFactory->Release();
    return S_OK;
}
