namespace AtmoWinRemoteControl
{
    using System;
    using System.Runtime.CompilerServices;
    using System.Runtime.InteropServices;

    [ComImport, TypeLibType((short) 0x100), Guid("E01E2047-5AFC-11D3-8E80-00805F91CDD9"), InterfaceType((short) 1)]
    public interface IAtmoLiveViewControl
    {
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void setLiveViewSource([In] ComLiveViewSource dwModus);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void getCurrentLiveViewSource(out ComLiveViewSource modus);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void setPixelData([In, MarshalAs(UnmanagedType.SafeArray, SafeArraySubType=VarEnum.VT_UI1)] Array bitmapInfoHeader, [In, MarshalAs(UnmanagedType.SafeArray, SafeArraySubType=VarEnum.VT_UI1)] Array pixelData);
    }
}

