namespace AtmoWinRemoteControl
{
  using System;
  using System.Runtime.CompilerServices;
  using System.Runtime.InteropServices;

  [ComImport, Guid("E01E2044-5AFC-11D3-8E80-00805F91CEE9"), ClassInterface((short)0), TypeLibType((short)2)]
  public class AtmoLiveViewControlClass : IAtmoLiveViewControl, AtmoLiveViewControl
  {
    [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
    public virtual extern void getCurrentLiveViewSource(out ComLiveViewSource modus);
    [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
    public virtual extern void setLiveViewSource([In] ComLiveViewSource dwModus);
    [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
    public virtual extern void setPixelData([In, MarshalAs(UnmanagedType.SafeArray,
      SafeArraySubType = VarEnum.VT_UI1)] Array bitmapInfoHeader, [In, MarshalAs(UnmanagedType.SafeArray,
        SafeArraySubType = VarEnum.VT_UI1)] Array pixelData);
  }
}

