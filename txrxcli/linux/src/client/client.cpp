#include <arpa/inet.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "client/client.hpp"
#include "txrxcli.hpp"

namespace txrx
{
    // addr, port, api_key, cmd
    Client::Client(std::string addr, uint16_t port, std::string api_key, std::string cmd)
        : addr_(std::move(addr)), port_(port), api_key_(std::move(api_key)), cmd_(std::move(cmd))
    {
    }

    Client::~Client()
    {
    }

    void Client::run()
    {
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC; // allow IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        addrinfo *res = nullptr; // head of a linked list of resolved addresses
        const std::string port_str = std::to_string(port_);
        int gai_rc = ::getaddrinfo(addr_.c_str(), port_str.c_str(), &hints, &res);
        if (gai_rc != 0)
        {
            txrx::err() << "hostname resolution failed: " << ::gai_strerror(gai_rc) << ANSI_RESET;
            return;
        }

        int sock = -1;
        for (addrinfo *rp = res; rp != nullptr; rp = rp->ai_next)
        {
            sock = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (sock < 0)
            {
                continue;
            }
            if (::connect(sock, rp->ai_addr, rp->ai_addrlen) == 0)
            {
                break; // successfully connected to the socket
            }
            ::close(sock);
            sock = -1;
        }
        ::freeaddrinfo(res);

        if (sock < 0)
        {
            txrx::perror("connect");
            return;
        }

        Request req{api_key_, cmd_};
        std::string frame = encode_request_frame(req);
        if (!send_buf_to_sock(sock, frame))
        {
            txrx::err() << "failed to send request" << ANSI_RESET;
            ::close(sock);
            return;
        }

        auto resp_or_err = decode_response_frame(sock);
        ::close(sock);

        if (auto ferr = std::get_if<FrameError>(&resp_or_err))
        {
            txrx::err() << "failed to decode response: " << ferr->message << ANSI_RESET;
            return;
        }

        const Response &resp = std::get<Response>(resp_or_err);
        std::cout << "Exit code: " << resp.exit_code << std::endl;
        if (!resp.stdout_text.empty())
        {
            std::cout << resp.stdout_text << std::endl;
        }
        if (!resp.stderr_text.empty())
        {
            std::cerr << resp.stderr_text << std::endl;
        }
    }
}
