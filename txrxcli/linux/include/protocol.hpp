#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace txrx
{

    /// Demo warning message reused by client and server.
    constexpr const char *kWarningBanner =
        "Demo-only remote execution. For educational purposes only. "
        "NOT FOR PRODUCTION! "
        "Never use outside of trusted, isolated networks.";

    /// Error frame returned when framing/decoding fails.
    struct FrameError
    {
        int code;
        std::string message;
    };

    /// Request frame carrying API key and command text.
    struct Request
    {
        std::string api_key;
        std::string command;
    };

    /// Response frame carrying exit code and captured output.
    struct Response
    {
        int exit_code{0};
        std::string stdout_text;
        std::string stderr_text;
    };

    /// HTTP-like status codes used in framing.
    inline constexpr int kStatusBadRequest = 400;
    inline constexpr int kStatusUnauthorized = 401;
    inline constexpr int kStatusInternalServerError = 500;

    /// Error messages used in framing errors.
    inline constexpr std::string_view kErrBadFraming = "bad framing";
    inline constexpr std::string_view kErrExpectedBlankLine = "expected blank line";
    inline constexpr std::string_view kErrInvalidApiKey = "invalid API key";
    inline constexpr std::string_view kErrInvalidCmdLen = "invalid command length";
    inline constexpr std::string_view kErrInvalidRespLen = "invalid response length(s)";
    inline constexpr std::string_view kErrMissingApiKey = "missing API key";
    inline constexpr std::string_view kErrMissingCmdLen = "missing command length";
    inline constexpr std::string_view kErrMissingExitCode = "missing exit code";
    inline constexpr std::string_view kErrMissingStdoutLen = "missing stdout length";
    inline constexpr std::string_view kErrMissingStderrLen = "missing stderr length";
    inline constexpr std::string_view kErrTruncatedCmd = "truncated command";
    inline constexpr std::string_view kErrTruncatedStderr = "truncated stderr";
    inline constexpr std::string_view kErrTruncatedStdout = "truncated stdout";

    /// Field labels used in request/response frames.
    inline constexpr std::string_view kFieldApiKey = "APIKEY:";
    inline constexpr std::string_view kFieldCommandLen = "COMMAND-LEN:";
    inline constexpr std::string_view kFieldExitCode = "EXIT-CODE:";
    inline constexpr std::string_view kFieldStderrLen = "STDERR-LEN:";
    inline constexpr std::string_view kFieldStdoutLen = "STDOUT-LEN:";

    /// Minimum header reservation for frame encoding.
    inline constexpr size_t kFrameHeaderBytesMin = 64;

    /// Receive exactly `n` bytes from `fd`, or return an empty string on error/EOF.
    /// @param fd socket file descriptor
    /// @param n number of bytes to read
    /// @return buffer of length n, or empty on failure/EOF
    std::string recv_n_bytes_from_sock(int fd, size_t n);
    /// Read a single line (terminated by '\n') from `fd`; trims trailing '\r' if present.
    /// @param fd socket file descriptor
    /// @return line text if read, std::nullopt on failure/EOF
    std::optional<std::string> recv_line_from_sock(int fd);
    /// Decode a framed request from a socket.
    /// @param fd socket file descriptor
    /// @return Request on success, FrameError on framing failure
    std::variant<Request, FrameError> decode_request_frame(int fd);
    /// Decode a framed response from a socket.
    /// @param fd socket file descriptor
    /// @return Response on success, FrameError on framing failure
    std::variant<Response, FrameError> decode_response_frame(int fd);
    /// Encode a Response into the wire format.
    /// @param r response to encode
    /// @return encoded buffer ready to send
    std::string encode_response_frame(const Response &r);
    /// Encode a Request into the wire format.
    /// @param req request to encode
    /// @return encoded buffer ready to send
    std::string encode_request_frame(const Request &req);
    /// Send the entire buffer to the socket, retrying until complete or failure.
    /// @param fd socket file descriptor
    /// @param buf data to send
    /// @return true on success, false on failure
    bool send_buf_to_sock(int fd, const std::string &buf);

} // namespace txrx
