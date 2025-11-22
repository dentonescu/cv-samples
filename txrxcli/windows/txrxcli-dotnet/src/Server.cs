using System.Net.Sockets;

namespace TxRxCli
{
    internal sealed class Server
    {
        private readonly int _port;
        private readonly string _apiKey;
        private readonly string _shell;

        public Server(int port, string apiKey, string shell)
        {
            _port = port;
            _apiKey = apiKey;
            _shell = shell;
        }

        /*
         * Helpers
         */
        private void HandleClient(TcpClient client)
        {
            using NetworkStream stream = client.GetStream();

            var (req, frameErr) = Protocol.DecodeRequest(stream);
            if (frameErr is not null)
            {
                var resp = new Protocol.Response(frameErr.Code, string.Empty, frameErr.Message);
                var buf = Protocol.EncodeResponse(resp);
                stream.Write(buf, 0, buf.Length);
                return;
            }

            if (req is null)
            {
                var resp = new Protocol.Response(Protocol.StatusInternalServerError, string.Empty, Protocol.ErrDecode);
                var buf = Protocol.EncodeResponse(resp);
                stream.Write(buf, 0, buf.Length);
                return;
            }

            if (!string.Equals(req.ApiKey, _apiKey, StringComparison.Ordinal))
            {
                var resp = new Protocol.Response(Protocol.StatusUnauthorized, string.Empty, Protocol.ErrInvalidApiKey);
                var buf = Protocol.EncodeResponse(resp);
                stream.Write(buf, 0, buf.Length);
                return;
            }

            // Execute command
            var (exitCode, stdout, stderr) = ProcessRunner.Run(_shell, req.Command);
            var response = new Protocol.Response(exitCode, stdout, stderr);
            var responseBuf = Protocol.EncodeResponse(response);
            stream.Write(responseBuf, 0, responseBuf.Length);
        }

        /*
         * Server loop
         */

        public void Run()
        {
            Common.Log(Common.WarningBanner);
            var listener = new TcpListener(System.Net.IPAddress.Any, _port);
            listener.Start();
            Common.Log($"Listening on port {_port} (shell={_shell}). Ctrl+C to stop.");

            while (true)
            {
                TcpClient? client = null;
                try
                {
                    client = listener.AcceptTcpClient();
                    HandleClient(client);
                }
                catch (Exception ex)
                {
                    Common.Err($"Server error: {ex.Message}");
                }
                finally
                {
                    client?.Close();
                }
            }
        }
    }
}
