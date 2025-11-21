#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace txrx
{

    // Demo warning message reused by client and server.
    constexpr const char *kWarningBanner =
        "Demo-only remote execution. For educational purposes only. "
        "NOT FOR PRODUCTION! "
        "Never use outside of trusted, isolated networks.";

    struct FrameError
    {
        int code;
        std::string message;
    };

    struct Request
    {
        std::string api_key;
        std::string command;
    };

    struct Response
    {
        int exit_code{0};
        std::string stdout_text;
        std::string stderr_text;
    };

    // status codes
    inline constexpr int kStatusBadRequest = 400;
    inline constexpr int kStatusUnauthorized = 401;
    inline constexpr int kStatusInternalServerError = 500;

    // error messages
    inline constexpr std::string_view kErrBadFraming = "bad framing";
    inline constexpr std::string_view kErrExpectedBlankLine = "expected blank line";
    inline constexpr std::string_view kErrInvalidApiKey = "invalid API key";
    inline constexpr std::string_view kErrInvalidCmdLen = "invalid command length";
    inline constexpr std::string_view kErrMissingApiKey = "missing API key";
    inline constexpr std::string_view kErrMissingCmdLen = "missing command length";
    inline constexpr std::string_view kErrTruncatedCmd = "truncated command";

    // frame fields
    inline constexpr std::string_view kFieldApiKey = "APIKEY:";
    inline constexpr std::string_view kFieldCommandLen = "COMMAND-LEN:";
    inline constexpr std::string_view kFieldExitCode = "EXIT-CODE:";
    inline constexpr std::string_view kFieldStderrLen = "STDERR-LEN:";
    inline constexpr std::string_view kFieldStdoutLen = "STDOUT-LEN:";

    // Protocol helpers (blocking, simple)
    std::string recv_n_bytes_from_sock(int fd, size_t n);
    std::optional<std::string> recv_line_from_sock(int fd);
    std::variant<Request, FrameError> decode_request_frame(int fd);
    std::string encode_response_frame(const Response &r);
    bool send_buf_to_sock(int fd, const std::string &buf);

} // namespace txrx
