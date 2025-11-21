#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include "ansi.hpp"
#include "protocol.hpp"
#include "server/server.hpp"
#include "txrxcli.hpp"

#define ARGS_MIN 3

namespace
{
    int cleanup(int status)
    {
        std::cout << std::endl
                  << std::endl;
        return status;
    }

    void print_usage()
    {
        std::cout << "\nUsage: txrxcli --mode server|client [options]";
        std::cout << "\n  server mode: --port <port> --key <api_key>";
        std::cout << "\n  client mode: (to be implemented) connects to server, sends API key + command, prints response";
    }

    void print_warning()
    {
        txrx::log() << txrx::kWarningBanner << ANSI_RESET;
    }

}

int main(int argc, char *argv[])
{
    print_warning();

    if (argc < ARGS_MIN)
    {
        print_usage();
        return cleanup(EXIT_FAILURE);
    }

    std::string mode_arg = argv[1];
    std::string mode_value = argv[2];

    if (mode_arg != "--mode")
    {
        print_usage();
        return cleanup(EXIT_FAILURE);
    }

    if (mode_value == "server")
    {
        uint16_t port = 0;
        std::string api_key;

        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--port" && i + 1 < argc)
            {
                port = static_cast<uint16_t>(std::stoi(argv[++i]));
            }
            else if (arg == "--key" && i + 1 < argc)
            {
                api_key = argv[++i];
            }
        }

        if (port == 0 || api_key.empty())
        {
            txrx::err() << "server mode requires --port and --key" << ANSI_RESET;
            print_usage();
            return cleanup(EXIT_FAILURE);
        }

        txrx::Server server(port, api_key);
        if (!server.start())
        {
            txrx::err() << "failed to start server" << ANSI_RESET;
            return cleanup(EXIT_FAILURE);
        }
        server.run();
    }
    else if (mode_value == "client")
    {
        txrx::log() << "Client mode placeholder. Socket and framing logic will follow." << ANSI_RESET;
        // TODO: implement client path
    }
    else
    {
        print_usage();
        return cleanup(EXIT_FAILURE);
    }

    return cleanup(EXIT_SUCCESS);
}
