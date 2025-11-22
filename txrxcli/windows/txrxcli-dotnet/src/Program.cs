using TxRxCli;

namespace TxRxCli
{
    internal static class Program
    {

        /*
         * Helpers
         */

        static void PrintUsage()
        {
            Console.WriteLine("Usage: txrxcli-dotnet --mode server|client [options]");
            Console.WriteLine("  server mode: --port <port> --key <api_key> [--shell powershell|cmd]");
            Console.WriteLine("  client mode: --host <addr> --port <port> --key <api_key> [--cmd <command>] [--shell powershell|cmd]");
            Console.WriteLine("  default shell: powershell");
        }

        /*
         * Main
         */

        static int Main(string[] args)
        {

            var argsList = new List<string>(args);

            string apiKey = "";
            string command = "";
            string host = "";
            string mode = "";
            string shell = Protocol.ShellPowershell;
            int port = 0;

            for (int i = 0; i < argsList.Count; ++i)
            {
                string arg = argsList[i];
                if (arg == "--mode" && i + 1 < argsList.Count)
                {
                    mode = argsList[++i];
                }
                else if (arg == "--host" && i + 1 < argsList.Count)
                {
                    host = argsList[++i];
                }
                else if (arg == "--port" && i + 1 < argsList.Count &&
                    int.TryParse(argsList[i + 1], out var parsedPort))
                {
                    port = parsedPort;
                    ++i;
                }
                else if (arg == "--key" && i + 1 < argsList.Count)
                {
                    apiKey = argsList[++i];
                }
                else if (arg == "--cmd" && i + 1 < argsList.Count)
                {
                    command = argsList[++i];
                }
                else if (arg == "--shell" && i + 1 < argsList.Count)
                {
                    shell = argsList[++i].ToLowerInvariant();
                }
            }

            if (string.IsNullOrWhiteSpace(mode))
            {
                PrintUsage();
                return Common.ExitFailure;
            }

            if (mode == "server")
            {
                if (port == 0 || string.IsNullOrWhiteSpace(apiKey))
                {
                    PrintUsage();
                    return Common.ExitFailure;
                }
                var server = new Server(port, apiKey, shell);
                server.Run();
            }
            else if (mode == "client")
            {
                if (port == 0 || string.IsNullOrWhiteSpace(apiKey) || string.IsNullOrWhiteSpace(host))
                {
                    PrintUsage();
                    return Common.ExitFailure;
                }

                if (string.IsNullOrWhiteSpace(command))
                {
                    Console.Write("Enter command: ");
                    command = Console.ReadLine() ?? string.Empty;
                }

                var client = new Client(host, port, apiKey, command, shell);
                client.Run();
            }
            else
            {
                PrintUsage();
                return Common.ExitFailure;
            }

            return Common.ExitSuccess;
        }
    }
}
