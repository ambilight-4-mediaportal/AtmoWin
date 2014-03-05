

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Wed Mar 05 15:09:05 2014
 */
/* Compiler settings for AtmoWin.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AtmoWin_h_h__
#define __AtmoWin_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAtmoRemoteControl_FWD_DEFINED__
#define __IAtmoRemoteControl_FWD_DEFINED__
typedef interface IAtmoRemoteControl IAtmoRemoteControl;

#endif 	/* __IAtmoRemoteControl_FWD_DEFINED__ */


#ifndef __IAtmoRemoteControl2_FWD_DEFINED__
#define __IAtmoRemoteControl2_FWD_DEFINED__
typedef interface IAtmoRemoteControl2 IAtmoRemoteControl2;

#endif 	/* __IAtmoRemoteControl2_FWD_DEFINED__ */


#ifndef __IAtmoLiveViewControl_FWD_DEFINED__
#define __IAtmoLiveViewControl_FWD_DEFINED__
typedef interface IAtmoLiveViewControl IAtmoLiveViewControl;

#endif 	/* __IAtmoLiveViewControl_FWD_DEFINED__ */


#ifndef __IAtmoLiveViewControl_FWD_DEFINED__
#define __IAtmoLiveViewControl_FWD_DEFINED__
typedef interface IAtmoLiveViewControl IAtmoLiveViewControl;

#endif 	/* __IAtmoLiveViewControl_FWD_DEFINED__ */


#ifndef __IAtmoRemoteControl_FWD_DEFINED__
#define __IAtmoRemoteControl_FWD_DEFINED__
typedef interface IAtmoRemoteControl IAtmoRemoteControl;

#endif 	/* __IAtmoRemoteControl_FWD_DEFINED__ */


#ifndef __IAtmoRemoteControl2_FWD_DEFINED__
#define __IAtmoRemoteControl2_FWD_DEFINED__
typedef interface IAtmoRemoteControl2 IAtmoRemoteControl2;

#endif 	/* __IAtmoRemoteControl2_FWD_DEFINED__ */


#ifndef __AtmoRemoteControl_FWD_DEFINED__
#define __AtmoRemoteControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class AtmoRemoteControl AtmoRemoteControl;
#else
typedef struct AtmoRemoteControl AtmoRemoteControl;
#endif /* __cplusplus */

#endif 	/* __AtmoRemoteControl_FWD_DEFINED__ */


#ifndef __AtmoLiveViewControl_FWD_DEFINED__
#define __AtmoLiveViewControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class AtmoLiveViewControl AtmoLiveViewControl;
#else
typedef struct AtmoLiveViewControl AtmoLiveViewControl;
#endif /* __cplusplus */

#endif 	/* __AtmoLiveViewControl_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_AtmoWin_0000_0000 */
/* [local] */ 


enum ComLiveViewSource
    {
        lvsGDI	= 0,
        lvsExternal	= 1
    } ;

enum ComEffectMode
    {
        cemUndefined	= -1,
        cemDisabled	= 0,
        cemStaticColor	= 1,
        cemLivePicture	= 2,
        cemColorChange	= 3,
        cemLrColorChange	= 4,
        cemColorMode	= 5
    } ;


extern RPC_IF_HANDLE __MIDL_itf_AtmoWin_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AtmoWin_0000_0000_v0_0_s_ifspec;

#ifndef __IAtmoRemoteControl_INTERFACE_DEFINED__
#define __IAtmoRemoteControl_INTERFACE_DEFINED__

/* interface IAtmoRemoteControl */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAtmoRemoteControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e01e2044-5afc-11d3-8e80-00805f91cdd9")
    IAtmoRemoteControl : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE setEffect( 
            /* [in] */ enum ComEffectMode dwEffect,
            /* [out] */ enum ComEffectMode *dwOldEffect) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE getEffect( 
            /* [in] */ enum ComEffectMode *dwEffect) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE setStaticColor( 
            /* [in] */ BYTE red,
            /* [in] */ BYTE green,
            /* [in] */ BYTE blue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE setChannelStaticColor( 
            /* [in] */ INT channel,
            /* [in] */ BYTE red,
            /* [in] */ BYTE green,
            /* [in] */ BYTE blue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE setChannelValues( 
            /* [in] */ SAFEARRAY * channel_values) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAtmoRemoteControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAtmoRemoteControl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAtmoRemoteControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAtmoRemoteControl * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setEffect )( 
            IAtmoRemoteControl * This,
            /* [in] */ enum ComEffectMode dwEffect,
            /* [out] */ enum ComEffectMode *dwOldEffect);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *getEffect )( 
            IAtmoRemoteControl * This,
            /* [in] */ enum ComEffectMode *dwEffect);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setStaticColor )( 
            IAtmoRemoteControl * This,
            /* [in] */ BYTE red,
            /* [in] */ BYTE green,
            /* [in] */ BYTE blue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setChannelStaticColor )( 
            IAtmoRemoteControl * This,
            /* [in] */ INT channel,
            /* [in] */ BYTE red,
            /* [in] */ BYTE green,
            /* [in] */ BYTE blue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setChannelValues )( 
            IAtmoRemoteControl * This,
            /* [in] */ SAFEARRAY * channel_values);
        
        END_INTERFACE
    } IAtmoRemoteControlVtbl;

    interface IAtmoRemoteControl
    {
        CONST_VTBL struct IAtmoRemoteControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAtmoRemoteControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAtmoRemoteControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAtmoRemoteControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAtmoRemoteControl_setEffect(This,dwEffect,dwOldEffect)	\
    ( (This)->lpVtbl -> setEffect(This,dwEffect,dwOldEffect) ) 

#define IAtmoRemoteControl_getEffect(This,dwEffect)	\
    ( (This)->lpVtbl -> getEffect(This,dwEffect) ) 

#define IAtmoRemoteControl_setStaticColor(This,red,green,blue)	\
    ( (This)->lpVtbl -> setStaticColor(This,red,green,blue) ) 

#define IAtmoRemoteControl_setChannelStaticColor(This,channel,red,green,blue)	\
    ( (This)->lpVtbl -> setChannelStaticColor(This,channel,red,green,blue) ) 

#define IAtmoRemoteControl_setChannelValues(This,channel_values)	\
    ( (This)->lpVtbl -> setChannelValues(This,channel_values) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAtmoRemoteControl_INTERFACE_DEFINED__ */


#ifndef __IAtmoRemoteControl2_INTERFACE_DEFINED__
#define __IAtmoRemoteControl2_INTERFACE_DEFINED__

/* interface IAtmoRemoteControl2 */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAtmoRemoteControl2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e01e2044-5bfc-11d4-8e80-00805f91cdd9")
    IAtmoRemoteControl2 : public IAtmoRemoteControl
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE getLiveViewRes( 
            /* [out] */ int *Width,
            /* [out] */ int *Height) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAtmoRemoteControl2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAtmoRemoteControl2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAtmoRemoteControl2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAtmoRemoteControl2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setEffect )( 
            IAtmoRemoteControl2 * This,
            /* [in] */ enum ComEffectMode dwEffect,
            /* [out] */ enum ComEffectMode *dwOldEffect);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *getEffect )( 
            IAtmoRemoteControl2 * This,
            /* [in] */ enum ComEffectMode *dwEffect);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setStaticColor )( 
            IAtmoRemoteControl2 * This,
            /* [in] */ BYTE red,
            /* [in] */ BYTE green,
            /* [in] */ BYTE blue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setChannelStaticColor )( 
            IAtmoRemoteControl2 * This,
            /* [in] */ INT channel,
            /* [in] */ BYTE red,
            /* [in] */ BYTE green,
            /* [in] */ BYTE blue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setChannelValues )( 
            IAtmoRemoteControl2 * This,
            /* [in] */ SAFEARRAY * channel_values);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *getLiveViewRes )( 
            IAtmoRemoteControl2 * This,
            /* [out] */ int *Width,
            /* [out] */ int *Height);
        
        END_INTERFACE
    } IAtmoRemoteControl2Vtbl;

    interface IAtmoRemoteControl2
    {
        CONST_VTBL struct IAtmoRemoteControl2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAtmoRemoteControl2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAtmoRemoteControl2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAtmoRemoteControl2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAtmoRemoteControl2_setEffect(This,dwEffect,dwOldEffect)	\
    ( (This)->lpVtbl -> setEffect(This,dwEffect,dwOldEffect) ) 

#define IAtmoRemoteControl2_getEffect(This,dwEffect)	\
    ( (This)->lpVtbl -> getEffect(This,dwEffect) ) 

#define IAtmoRemoteControl2_setStaticColor(This,red,green,blue)	\
    ( (This)->lpVtbl -> setStaticColor(This,red,green,blue) ) 

#define IAtmoRemoteControl2_setChannelStaticColor(This,channel,red,green,blue)	\
    ( (This)->lpVtbl -> setChannelStaticColor(This,channel,red,green,blue) ) 

#define IAtmoRemoteControl2_setChannelValues(This,channel_values)	\
    ( (This)->lpVtbl -> setChannelValues(This,channel_values) ) 


#define IAtmoRemoteControl2_getLiveViewRes(This,Width,Height)	\
    ( (This)->lpVtbl -> getLiveViewRes(This,Width,Height) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAtmoRemoteControl2_INTERFACE_DEFINED__ */


#ifndef __IAtmoLiveViewControl_INTERFACE_DEFINED__
#define __IAtmoLiveViewControl_INTERFACE_DEFINED__

/* interface IAtmoLiveViewControl */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAtmoLiveViewControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e01e2047-5afc-11d3-8e80-00805f91cdd9")
    IAtmoLiveViewControl : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE setLiveViewSource( 
            /* [in] */ enum ComLiveViewSource dwModus) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE getCurrentLiveViewSource( 
            /* [out] */ enum ComLiveViewSource *modus) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE setPixelData( 
            /* [in] */ SAFEARRAY * bitmapInfoHeader,
            /* [in] */ SAFEARRAY * pixelData) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAtmoLiveViewControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAtmoLiveViewControl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAtmoLiveViewControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAtmoLiveViewControl * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setLiveViewSource )( 
            IAtmoLiveViewControl * This,
            /* [in] */ enum ComLiveViewSource dwModus);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *getCurrentLiveViewSource )( 
            IAtmoLiveViewControl * This,
            /* [out] */ enum ComLiveViewSource *modus);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *setPixelData )( 
            IAtmoLiveViewControl * This,
            /* [in] */ SAFEARRAY * bitmapInfoHeader,
            /* [in] */ SAFEARRAY * pixelData);
        
        END_INTERFACE
    } IAtmoLiveViewControlVtbl;

    interface IAtmoLiveViewControl
    {
        CONST_VTBL struct IAtmoLiveViewControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAtmoLiveViewControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAtmoLiveViewControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAtmoLiveViewControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAtmoLiveViewControl_setLiveViewSource(This,dwModus)	\
    ( (This)->lpVtbl -> setLiveViewSource(This,dwModus) ) 

#define IAtmoLiveViewControl_getCurrentLiveViewSource(This,modus)	\
    ( (This)->lpVtbl -> getCurrentLiveViewSource(This,modus) ) 

#define IAtmoLiveViewControl_setPixelData(This,bitmapInfoHeader,pixelData)	\
    ( (This)->lpVtbl -> setPixelData(This,bitmapInfoHeader,pixelData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAtmoLiveViewControl_INTERFACE_DEFINED__ */



#ifndef __AtmoWinRemoteControl_LIBRARY_DEFINED__
#define __AtmoWinRemoteControl_LIBRARY_DEFINED__

/* library AtmoWinRemoteControl */
/* [version][helpstring][uuid] */ 





EXTERN_C const IID LIBID_AtmoWinRemoteControl;

EXTERN_C const CLSID CLSID_AtmoRemoteControl;

#ifdef __cplusplus

class DECLSPEC_UUID("e01e2045-5afc-11d3-8e80-00805f91cee9")
AtmoRemoteControl;
#endif

EXTERN_C const CLSID CLSID_AtmoLiveViewControl;

#ifdef __cplusplus

class DECLSPEC_UUID("e01e2044-5afc-11d3-8e80-00805f91cee9")
AtmoLiveViewControl;
#endif
#endif /* __AtmoWinRemoteControl_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


