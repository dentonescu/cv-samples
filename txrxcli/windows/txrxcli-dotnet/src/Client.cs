using System.Net.Sockets;

namespace TxRxCli
{
    internal sealed class Client
    {
        private readonly string _host;
        private readonly int _port;
        private readonly string _apiKey;
        private readonly string _command;
        private readonly string _shell;

        public Client(string host, int port, string apiKey, string command, string shell)
        {
            _host = host;
            _port = port;
            _apiKey = apiKey;
            _command = command;
            _shell = shell;
        }

        public void Run()
        {
            Common.Log(Common.WarningBanner);
            Common.Log($"Connecting to {_host}:{_port} (shell={_shell})...");

            try
            {
                using var client = new TcpClient();
                client.Connect(_host, _port);
                using NetworkStream stream = client.GetStream();

                var req = new Protocol.Request(_apiKey, _command);
                var requestBytes = Protocol.EncodeRequest(req);
                stream.Write(requestBytes, 0, requestBytes.Length);

                var (resp, err) = Protocol.DecodeResponse(stream);
                if (err is not null)
                {
                    Common.Err($"Failed to decode response: {err.Message}");
                    return;
                }
                if (resp is null)
                {
                    Common.Err("Unknown response decode error.");
                    return;
                }

                Common.Log($"Exit code: {resp.ExitCode}");
                if (!string.IsNullOrEmpty(resp.Stdout))
                {
                    Console.WriteLine(resp.Stdout);
                }
                if (!string.IsNullOrEmpty(resp.Stderr))
                {
                    Console.Error.WriteLine(resp.Stderr);
                }
            }
            catch (Exception ex)
            {
                Common.Err($"Client error: {ex.Message}");
            }
        }
    }
}
