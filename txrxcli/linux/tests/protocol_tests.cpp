#include <unistd.h>
#include <string>
#include <variant>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "protocol.hpp"

namespace txrx
{
    constexpr std::string_view kSecret = "not_the_real_api_key";

    namespace
    {
        int make_socket_pair(int sv[2])
        {
            spdlog::debug("make_socket_pair() started.");
            // create a pair of connected UNIX-domain sockets sv[0] and sv[1]
            // for in-memory send/recv without binding to a real TCP port
            int ret = ::socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
            spdlog::debug("make_socket_pair() ended. ret={}", ret);
            return ret;
        }
    }
}

TEST(ProtocolHelpers, DecodeRequestFrameSuccess)
{
    spdlog::set_level(spdlog::level::debug);
    int sv[2];
    ASSERT_EQ(txrx::make_socket_pair(sv), 0);
    std::string frame;
    constexpr std::string_view kCmd = "echo";
    frame += std::string(txrx::kFieldApiKey) + std::string{txrx::kSecret} + "\n";
    frame += std::string(txrx::kFieldCommandLen) + std::to_string(kCmd.size()) + "\n\n";
    frame += std::string{kCmd};
    ASSERT_EQ(::send(sv[0], frame.data(), frame.size(), 0), static_cast<ssize_t>(frame.size()));
    auto parsed = txrx::decode_request_frame(sv[1]);
    ASSERT_TRUE(std::holds_alternative<txrx::Request>(parsed));
    const auto &req = std::get<txrx::Request>(parsed);
    EXPECT_EQ(req.api_key, std::string{txrx::kSecret});
    EXPECT_EQ(req.command, std::string{kCmd});
    ::close(sv[0]);
    ::close(sv[1]);
}

TEST(ProtocolHelpers, EncodeResponseFrame)
{
    txrx::Response r;
    r.exit_code = 200;
    r.stdout_text = "Everything is awesome!";
    r.stderr_text = "There weren't any errors!";
    std::string encoded = txrx::encode_response_frame(r);
    std::string expected_header =
        std::string(txrx::kFieldExitCode) + std::to_string(r.exit_code) + "\n" +
        std::string(txrx::kFieldStdoutLen) + std::to_string(r.stdout_text.size()) + "\n" +
        std::string(txrx::kFieldStderrLen) + std::to_string(r.stderr_text.size()) + "\n\n";
    ASSERT_GE(encoded.size(), expected_header.size());
    EXPECT_EQ(encoded.substr(0, expected_header.size()), expected_header);
    EXPECT_NE(encoded.find("awesome"), std::string::npos);
    EXPECT_NE(encoded.find("weren't"), std::string::npos);
}
