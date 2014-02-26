namespace AtmoWinRemoteControl
{
    using System;
    using System.Runtime.CompilerServices;
    using System.Runtime.InteropServices;

    [ComImport, InterfaceType((short) 1), Guid("E01E2044-5AFC-11D3-8E80-00805F91CDD9"), TypeLibType((short) 0x100)]
    public interface IAtmoRemoteControl
    {
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void setEffect([In] ComEffectMode dwEffect, out ComEffectMode dwOldEffect);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void getEffect([In] ref ComEffectMode dwEffect);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void setStaticColor([In] byte red, [In] byte green, [In] byte blue);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void setChannelStaticColor([In] int channel, [In] byte red, [In] byte green, [In] byte blue);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
        void setChannelValues([In, MarshalAs(UnmanagedType.SafeArray, SafeArraySubType=VarEnum.VT_UI1)] Array channel_values);
    }
}

