#pragma once

#include <cstdint>
#include <string>

#include "protocol.hpp"

namespace txrx
{
    class Client
    {
    public:
        Client(std::string addr, uint16_t port, std::string api_key, std::string cmd);
        ~Client();

        void run();

    private:
        std::string addr_;
        uint16_t port_;
        std::string api_key_;
        std::string cmd_;
    };

} // namespace txrx
