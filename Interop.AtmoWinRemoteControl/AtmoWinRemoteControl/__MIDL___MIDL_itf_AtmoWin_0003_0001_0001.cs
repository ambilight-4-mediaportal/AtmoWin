namespace AtmoWinRemoteControl
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential, Pack=4)]
    public struct __MIDL___MIDL_itf_AtmoWin_0003_0001_0001
    {
        public uint Data1;
        public ushort Data2;
        public ushort Data3;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst=8)]
        public byte[] Data4;
    }
}

