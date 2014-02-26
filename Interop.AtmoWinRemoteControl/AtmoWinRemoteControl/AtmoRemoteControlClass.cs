namespace AtmoWinRemoteControl
{
    using System;
    using System.Runtime.CompilerServices;
    using System.Runtime.InteropServices;

    [ComImport, TypeLibType((short) 2), ClassInterface((short) 0), Guid("E01E2045-5AFC-11D3-8E80-00805F91CEE9")]
    public class AtmoRemoteControlClass : IAtmoRemoteControl, AtmoRemoteControl, IAtmoRemoteControl2
    {
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void getEffect([In] ref ComEffectMode dwEffect);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void getLiveViewRes(out int Width, out int Height);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void IAtmoRemoteControl2_getEffect([In] ref ComEffectMode dwEffect);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void IAtmoRemoteControl2_setChannelStaticColor([In] int channel, [In] byte red, [In] byte green, [In] byte blue);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void IAtmoRemoteControl2_setChannelValues([In, MarshalAs(UnmanagedType.SafeArray, SafeArraySubType=VarEnum.VT_UI1)] Array channel_values);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void IAtmoRemoteControl2_setEffect([In] ComEffectMode dwEffect, out ComEffectMode dwOldEffect);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void IAtmoRemoteControl2_setStaticColor([In] byte red, [In] byte green, [In] byte blue);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void setChannelStaticColor([In] int channel, [In] byte red, [In] byte green, [In] byte blue);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void setChannelValues([In, MarshalAs(UnmanagedType.SafeArray, SafeArraySubType=VarEnum.VT_UI1)] Array channel_values);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void setEffect([In] ComEffectMode dwEffect, out ComEffectMode dwOldEffect);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        public virtual extern void setStaticColor([In] byte red, [In] byte green, [In] byte blue);
    }
}

