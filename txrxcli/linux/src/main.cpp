#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include "ansi.hpp"
#include "protocol.hpp"
#include "client/client.hpp"
#include "server/server.hpp"
#include "txrxcli.hpp"

#define ARGS_MIN 3

namespace
{
    const std::string_view kParamMode = "--mode";
    const std::string_view kParamPort = "--port";
    const std::string_view kParamHost = "--host";
    const std::string_view kParamKey = "--key";

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
        std::cout << "\n  client mode: --host <address> --port <port>"
                  << " --key <api_key> --cmd <command>";
    }

    void print_warning()
    {
        txrx::log() << txrx::kWarningBanner << ANSI_RESET << std::endl;
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

    if (mode_arg != std::string(kParamMode))
    {
        print_usage();
        return cleanup(EXIT_FAILURE);
    }

    if (mode_value == "server")
    {
        uint16_t port = 0;
        std::string api_key;

        // server options
        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == std::string(kParamPort) && i + 1 < argc)
            {
                port = static_cast<uint16_t>(std::stoi(argv[++i]));
            }
            else if (arg == std::string(kParamKey) && i + 1 < argc)
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

        // server launcher
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
        uint16_t port = 0;
        std::string addr;
        std::string api_key;
        std::string cmd;

        // client options
        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == std::string(kParamPort) && i + 1 < argc)
            {
                port = static_cast<uint16_t>(std::stoi(argv[++i]));
            }
            else if (arg == std::string(kParamKey) && i + 1 < argc)
            {
                api_key = argv[++i];
            }
            else if (arg == std::string(kParamHost) && i + 1 < argc)
            {
                addr = argv[++i];
            }
            else if (arg == "--cmd" && i + 1 < argc)
            {
                cmd = argv[++i];
            }
        }

        if (cmd.empty())
        {
            std::cout << "Enter command: " << std::flush;
            std::getline(std::cin, cmd);
        }

        if (port == 0 || api_key.empty() || addr.empty() || cmd.empty())
        {
            txrx::err() << "client mode requires --host, --port, --key and --cmd" << ANSI_RESET;
            print_usage();
            return cleanup(EXIT_FAILURE);
        }

        // client launcher
        txrx::Client client(addr, port, api_key, cmd);
        client.run();
    }
    else
    {
        print_usage();
        return cleanup(EXIT_FAILURE);
    }

    return cleanup(EXIT_SUCCESS);
}
