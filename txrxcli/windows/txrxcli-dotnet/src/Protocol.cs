using System.Text;

namespace TxRxCli
{
    /// <summary>
    /// Framing helpers and constants for txrxcli.
    /// </summary>
    internal static class Protocol
    {

        public const string ShellPowershell = "powershell";
        public const string ShellCmd = "cmd";

        // Status codes (HTTP-like)
        public const int StatusBadRequest = 400;
        public const int StatusUnauthorized = 401;
        public const int StatusInternalServerError = 500;

        // Error messages
        public const string ErrBadFraming = "bad framing";
        public const string ErrDecode = "decode error";
        public const string ErrExpectedBlankLine = "expected blank line";
        public const string ErrMissingApiKey = "missing API key";
        public const string ErrMissingCommandLength = "missing command length";
        public const string ErrInvalidApiKey = "invalid API key";
        public const string ErrInvalidCommandLength = "invalid command length";
        public const string ErrTruncatedCommand = "truncated command";
        public const string ErrMissingExitCode = "missing exit code";
        public const string ErrMissingStdoutLength = "missing stdout length";
        public const string ErrMissingStderrLength = "missing stderr length";
        public const string ErrInvalidExitCode = "invalid exit code";
        public const string ErrInvalidStdoutLength = "invalid stdout length";
        public const string ErrInvalidStderrLength = "invalid stderr length";
        public const string ErrTruncatedStdout = "truncated stdout";
        public const string ErrTruncatedStderr = "truncated stderr";

        // Frame fields
        public const string FieldApiKey = "APIKEY:";
        public const string FieldCommandLen = "COMMAND-LEN:";
        public const string FieldExitCode = "EXIT-CODE:";
        public const string FieldStdoutLen = "STDOUT-LEN:";
        public const string FieldStderrLen = "STDERR-LEN:";


        private const int HeaderReserveBytes = 64;
        private const int InitialLineCapacity = 64;

        private static readonly Encoding Utf8 = new UTF8Encoding(false);

        public record FrameError(int Code, string Message);
        public record Request(string ApiKey, string Command);
        public record Response(int ExitCode, string Stdout, string Stderr);

        /*
         * Helpers
         */
        private static byte[] Combine(params byte[][] buffers)
        {
            var total = buffers.Sum(b => b.Length);
            var result = new byte[total];
            int offset = 0;
            foreach (var buf in buffers)
            {
                Buffer.BlockCopy(buf, 0, result, offset, buf.Length);
                offset += buf.Length;
            }
            return result;
        }

        private static string? ReadLineFromStream(Stream stream)
        {
            var bytes = new List<byte>(InitialLineCapacity);
            while (true)
            {
                int b = stream.ReadByte();
                if (b == -1)
                {
                    return null;
                }
                if (b == '\n')
                {
                    // Trim optional '\r'
                    if (bytes.Count > 0 && bytes[^1] == '\r')
                    {
                        bytes.RemoveAt(bytes.Count - 1);
                    }
                    return Utf8.GetString(bytes.ToArray());
                }
                bytes.Add((byte)b);
            }
        }

        private static byte[]? ReadExactCountFromStream(Stream stream, int count)
        {
            if (count == 0)
            {
                return Array.Empty<byte>();
            }
            var buffer = new byte[count];
            int offset = 0;
            while (offset < count)
            {
                int read = stream.Read(buffer, offset, count - offset);
                if (read <= 0)
                {
                    return null;
                }
                offset += read;
            }
            return buffer;
        }

        /*
         * Externals
         */

        /// <summary>
        /// Encode a request frame into bytes.<br/><br/>
        /// <strong>Request framing</strong><br/>
        /// APIKEY:(key)\n<br/>
        /// COMMAND-LEN:(number of bytes)\n<br/>
        /// \n<br/>
        /// (command bytes)<br/>
        /// </summary>
        public static byte[] EncodeRequest(Request req)
        {
            var commandBytes = Utf8.GetBytes(req.Command);
            var sb = new StringBuilder(FieldApiKey.Length + req.ApiKey.Length + HeaderReserveBytes);
            sb.Append(FieldApiKey).Append(req.ApiKey).Append('\n');
            sb.Append(FieldCommandLen).Append(commandBytes.Length).Append("\n\n");
            var headerBytes = Utf8.GetBytes(sb.ToString());
            return Combine(headerBytes, commandBytes);
        }

        /// <summary>
        /// Encode a response frame into bytes.<br/><br/>
        /// <strong>Response framing</strong><br/>
        /// EXIT-CODE:(int)\n<br/>
        /// STDOUT-LEN:(number of bytes)\n<br/>
        /// STDERR-LEN:(number of bytes)\n<br/>
        /// \n<br/>
        /// (stdout bytes)(stderr bytes)<br/>
        /// </summary>
        public static byte[] EncodeResponse(Response resp)
        {
            var stdoutBytes = Utf8.GetBytes(resp.Stdout);
            var stderrBytes = Utf8.GetBytes(resp.Stderr);
            var sb = new StringBuilder(HeaderReserveBytes);
            sb.Append(FieldExitCode).Append(resp.ExitCode).Append('\n');
            sb.Append(FieldStdoutLen).Append(stdoutBytes.Length).Append('\n');
            sb.Append(FieldStderrLen).Append(stderrBytes.Length).Append("\n\n");
            var headerBytes = Utf8.GetBytes(sb.ToString());
            return Combine(headerBytes, stdoutBytes, stderrBytes);
        }

        /// <summary>
        /// Decode a request frame from a stream.
        /// </summary>
        public static (Request? request, FrameError? error) DecodeRequest(Stream stream)
        {
            var apiLine = ReadLineFromStream(stream);
            var lenLine = ReadLineFromStream(stream);
            var blankLine = ReadLineFromStream(stream);
            if (apiLine is null || lenLine is null || blankLine is null)
            {
                return (null, new FrameError(StatusBadRequest, ErrBadFraming));
            }
            if (blankLine.Length != 0)
            {
                return (null, new FrameError(StatusBadRequest, ErrExpectedBlankLine));
            }
            if (!apiLine.StartsWith(FieldApiKey, StringComparison.Ordinal))
            {
                return (null, new FrameError(StatusBadRequest, ErrMissingApiKey));
            }
            if (!lenLine.StartsWith(FieldCommandLen, StringComparison.Ordinal))
            {
                return (null, new FrameError(StatusBadRequest, ErrMissingCommandLength));
            }

            var apiKey = apiLine.Substring(FieldApiKey.Length);
            if (!int.TryParse(lenLine.AsSpan(FieldCommandLen.Length), out var cmdLen) || cmdLen < 0)
            {
                return (null, new FrameError(StatusBadRequest, ErrInvalidCommandLength));
            }

            var cmdBytes = ReadExactCountFromStream(stream, cmdLen);
            if (cmdBytes is null)
            {
                return (null, new FrameError(StatusBadRequest, ErrTruncatedCommand));
            }

            var command = Utf8.GetString(cmdBytes);
            return (new Request(apiKey, command), null);
        }

        /// <summary>
        /// Decode a response frame from a stream.
        /// </summary>
        public static (Response? response, FrameError? error) DecodeResponse(Stream stream)
        {
            var exitLine = ReadLineFromStream(stream);
            var outLenLine = ReadLineFromStream(stream);
            var errLenLine = ReadLineFromStream(stream);
            var blankLine = ReadLineFromStream(stream);
            if (exitLine is null || outLenLine is null || errLenLine is null || blankLine is null)
            {
                return (null, new FrameError(StatusBadRequest, ErrBadFraming));
            }
            if (blankLine.Length != 0)
            {
                return (null, new FrameError(StatusBadRequest, ErrExpectedBlankLine));
            }
            if (!exitLine.StartsWith(FieldExitCode, StringComparison.Ordinal))
            {
                return (null, new FrameError(StatusBadRequest, ErrMissingExitCode));
            }
            if (!outLenLine.StartsWith(FieldStdoutLen, StringComparison.Ordinal))
            {
                return (null, new FrameError(StatusBadRequest, ErrMissingStdoutLength));
            }
            if (!errLenLine.StartsWith(FieldStderrLen, StringComparison.Ordinal))
            {
                return (null, new FrameError(StatusBadRequest, ErrMissingStderrLength));
            }

            if (!int.TryParse(exitLine.AsSpan(FieldExitCode.Length), out var exitCode))
            {
                return (null, new FrameError(StatusBadRequest, ErrInvalidExitCode));
            }
            if (!int.TryParse(outLenLine.AsSpan(FieldStdoutLen.Length), out var stdoutLen) || stdoutLen < 0)
            {
                return (null, new FrameError(StatusBadRequest, ErrInvalidStdoutLength));
            }
            if (!int.TryParse(errLenLine.AsSpan(FieldStderrLen.Length), out var stderrLen) || stderrLen < 0)
            {
                return (null, new FrameError(StatusBadRequest, ErrInvalidStderrLength));
            }

            var stdoutBytes = ReadExactCountFromStream(stream, stdoutLen);
            if (stdoutBytes is null)
            {
                return (null, new FrameError(StatusBadRequest, ErrTruncatedStdout));
            }
            var stderrBytes = ReadExactCountFromStream(stream, stderrLen);
            if (stderrBytes is null)
            {
                return (null, new FrameError(StatusBadRequest, ErrTruncatedStderr));
            }

            return (new Response(exitCode, Utf8.GetString(stdoutBytes), Utf8.GetString(stderrBytes)), null);
        }
    }
}
