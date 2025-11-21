#pragma once

#include <cstdint>
#include <string>

#include "protocol.hpp"

#define MAX_PENDING_CONNECTIONS 8

namespace txrx
{
    class Server
    {
    public:
        Server(uint16_t port, std::string api_key);
        ~Server();

        bool start();
        void run();

    private:
        void handle_client(int fd);
        Response execute_command(const std::string &command);
        static std::string read_fd_to_string(int fd);

        uint16_t port_;
        std::string api_key_;
        int listen_fd_;
    };

} // namespace txrx
