#include "protocol.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace txrx
{

    std::string recv_n_bytes_from_sock(int fd, size_t n)
    {
        std::string out;
        out.resize(n);
        size_t offset = 0;

        while (offset < n)
        {
            ssize_t got = ::recv(fd, &out[offset], n - offset, 0);
            if (got <= 0)
            {
                // we expect n bytes so clear out everything if we end up with less than expected
                out.clear();
                break;
            }
            offset += static_cast<size_t>(got);
        }

        return out;
    }

    std::optional<std::string> recv_line_from_sock(int fd)
    {
        std::string line;
        char ch;

        while (true)
        {
            ssize_t got = ::recv(fd, &ch, 1, 0); // read one byte at a time
            if (got <= 0)
            {
                return std::nullopt;
            }
            if (ch == '\n')
            {
                // strip optional carriage return
                if (!line.empty() && line.back() == '\r')
                {
                    line.pop_back();
                }
                return line;
            }
            line.push_back(ch);
        }
    }

    std::variant<Request, FrameError> decode_request_frame(int fd)
    {
        /*
         * Request frame
         * =============
         * APIKEY:<key>\n
         * COMMAND-LEN:<bytes>\n
         * \n
         * <command bytes>
         */
        auto api_line = recv_line_from_sock(fd);
        auto len_line = recv_line_from_sock(fd);
        auto blank = recv_line_from_sock(fd);

        if (!api_line || !len_line || !blank)
        {
            return FrameError{static_cast<int>(kStatusBadRequest), std::string{kErrBadFraming}};
        }
        if (!blank->empty())
        {
            return FrameError{static_cast<int>(kStatusBadRequest), std::string{kErrExpectedBlankLine}};
        }

        constexpr std::string_view kApiPrefix = kFieldApiKey;
        constexpr std::string_view kCmdLenPrefix = kFieldCommandLen;

        if (api_line->compare(0, kApiPrefix.size(), kApiPrefix) != 0)
        {
            return FrameError{static_cast<int>(kStatusBadRequest), std::string{kErrMissingApiKey}};
        }
        if (len_line->compare(0, kCmdLenPrefix.size(), kCmdLenPrefix) != 0)
        {
            return FrameError{static_cast<int>(kStatusBadRequest), std::string{kErrMissingCmdLen}};
        }

        std::string api_key = api_line->substr(kApiPrefix.size());
        size_t cmd_len = 0;
        try
        {
            cmd_len = static_cast<size_t>(std::stoul(len_line->substr(kCmdLenPrefix.size())));
        }
        catch (...)
        {
            return FrameError{static_cast<int>(kStatusBadRequest), std::string{kErrInvalidCmdLen}};
        }

        std::string cmd = recv_n_bytes_from_sock(fd, cmd_len);
        if (cmd.size() != cmd_len)
        {
            return FrameError{static_cast<int>(kStatusBadRequest), std::string{kErrTruncatedCmd}};
        }

        return Request{std::move(api_key), std::move(cmd)};
    }

    std::string encode_response_frame(const Response &r)
    {
        /*
         * EXIT-CODE:<int>\n
         * STDOUT-LEN:<bytes>\n
         * STDERR-LEN:<bytes>\n
         * \n
         * <stdout bytes><stderr bytes>
         */
        std::string out;
        constexpr size_t kHeaderMin = 64;
        out.reserve(kHeaderMin + r.stdout_text.size() + r.stderr_text.size());
        out += kFieldExitCode;
        out += std::to_string(r.exit_code);
        out += "\n";
        out += kFieldStdoutLen;
        out += std::to_string(r.stdout_text.size());
        out += "\n";
        out += kFieldStderrLen;
        out += std::to_string(r.stderr_text.size());
        out += "\n\n";
        out += r.stdout_text;
        out += r.stderr_text;
        return out;
    }

    bool send_buf_to_sock(int fd, const std::string &buf)
    {
        size_t offset = 0;
        while (offset < buf.size())
        {
            ssize_t sent = ::send(fd, buf.data() + offset, buf.size() - offset, 0);
            if (sent <= 0)
            {
                return false;
            }
            offset += static_cast<size_t>(sent);
        }
        return true;
    }

} // namespace txrx
