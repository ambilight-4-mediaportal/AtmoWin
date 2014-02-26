namespace AtmoWinRemoteControl
{
    using System.Runtime.InteropServices;

    [ComImport, Guid("E01E2044-5AFC-11D3-8E80-00805F91CDD9"), CoClass(typeof(AtmoRemoteControlClass))]
    public interface AtmoRemoteControl : IAtmoRemoteControl
    {
    }
}

