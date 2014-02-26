namespace AtmoWinRemoteControl
{
    using System.Runtime.InteropServices;

    [ComImport, Guid("E01E2047-5AFC-11D3-8E80-00805F91CDD9"), CoClass(typeof(AtmoLiveViewControlClass))]
    public interface AtmoLiveViewControl : IAtmoLiveViewControl
    {
    }
}

