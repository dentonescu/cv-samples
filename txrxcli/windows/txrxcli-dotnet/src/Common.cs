using TxRxCli;

namespace TxRxCli
{
    internal static class Common
    {
        public const string WarningBanner =
            "Demo-only remote execution. For educational purposes only. "
            + "NOT FOR PRODUCTION! Never use outside of trusted, isolated networks.";

        private static readonly object _consoleLock = new();
        public const int ExitSuccess = 0;
        public const int ExitFailure = 1;

        public static void Err(string message)
        {
            lock (_consoleLock)
            {
                var old = Console.ForegroundColor;
                Console.ForegroundColor = ConsoleColor.Red;
                Console.Error.WriteLine($"[txrxcli] {message}");
                Console.ForegroundColor = old;
            }
        }

        public static void Log(string message)
        {
            lock (_consoleLock)
            {
                var old = Console.ForegroundColor;
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine($"[txrxcli] {message}");
                Console.ForegroundColor = old;
            }
        }
    }
}
