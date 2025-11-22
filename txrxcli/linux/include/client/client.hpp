#pragma once

#include <cstdint>
#include <string>

#include "protocol.hpp"

namespace txrx
{
    /// TCP client that frames requests, validates API key, and prints server responses.
    class Client
    {
    public:
        /// Construct a client targeting host/port with API key and command.
        Client(std::string addr, uint16_t port, std::string api_key, std::string cmd);
        ~Client();

        /// Connect to the server, send framed request, and print response.
        /// @return void
        void run();

    private:
        std::string addr_;
        uint16_t port_;
        std::string api_key_;
        std::string cmd_;
    };

} // namespace txrx
