#pragma once

#include "ansi.hpp"
#include <cstring>

namespace txrx
{
    inline std::string kLogPrefix = "[txrxcli] ";

    inline std::ostream &err()
    {
        return std::cerr << ANSI_RESET << ANSI_RED << "\n"
                         << kLogPrefix;
    }

    inline std::ostream &log()
    {
        return std::cout << ANSI_RESET << ANSI_GREEN << "\n"
                         << kLogPrefix;
    }

    inline std::ostream &perror(std::string msg)
    {
        return txrx::err() << msg << ": " << std::strerror(errno) << ANSI_RESET;
    }
}