using System.IO;
using Xunit;
using Xunit.Abstractions;
using static TxRxCli.Protocol;

namespace TxRxCli.Tests
{
    public class ProtocolTests
    {
        public ProtocolTests(ITestOutputHelper output) => _output = output;

        private readonly ITestOutputHelper _output;

        protected const string ApiKey = "not-the-real-api-key";

        [Fact]
        public void EncodeDecodeRequest_Succeeds()
        {
            var req = new Request(ApiKey, "echo hello");
            _output.WriteLine($"req: {req}");
            var bytes = EncodeRequest(req);
            using var ms = new MemoryStream(bytes); // test without sockets
            var (decoded, err) = DecodeRequest(ms);
            Assert.Null(err);
            Assert.NotNull(decoded);
            Assert.Equal(req.ApiKey, decoded!.ApiKey);
            Assert.Equal(req.Command, decoded.Command);
        }

        [Fact]
        public void EncodeDecodeResponse_Succeeds()
        {
            var resp = new Response(42,
                "This is the output of some command",
                "Here's the error text which appeared");
            _output.WriteLine($"resp: {resp}");
            var bytes = EncodeResponse(resp);
            using var ms = new MemoryStream(bytes); // test without sockets
            var (decoded, err) = DecodeResponse(ms);
            Assert.Null(err);
            Assert.NotNull(decoded);
            Assert.Equal(resp.ExitCode, decoded!.ExitCode);
            Assert.Equal(resp.Stdout, decoded.Stdout);
            Assert.Equal(resp.Stderr, decoded.Stderr);
        }

        [Fact]
        public void DecodeRequest_Truncated_Command_Fails()
        {
            var req = new Request(ApiKey, "ping example.com");
            _output.WriteLine($"req: {req}");
            var bytes = EncodeRequest(req);
            // drop some bytes to simulate truncation
            var truncated = bytes[..(bytes.Length - 2)];
            using var ms = new MemoryStream(truncated);
            var (decoded, err) = DecodeRequest(ms);
            Assert.Null(decoded);
            Assert.NotNull(err);
            Assert.Equal(StatusBadRequest, err!.Code);
        }

        [Fact]
        public void DecodeResponse_Truncated_Output_Fails()
        {
            var resp = new Response(0,
                "output of the last command",
                "error message from the last command");
            _output.WriteLine($"resp: {resp}");
            var bytes = EncodeResponse(resp);
            // drop some bytes to simulate truncation
            var truncated = bytes[..(bytes.Length - 2)];
            using var ms = new MemoryStream(truncated);
            var (decoded, err) = DecodeResponse(ms);
            Assert.Null(decoded);
            Assert.NotNull(err);
            Assert.Equal(StatusBadRequest, err!.Code);
        }
    }
}
