#pragma once

#include <cstdint>
#include <string>

#include "protocol.hpp"

#define MAX_PENDING_CONNECTIONS 8

namespace txrx
{
    /// Blocking TCP server that validates API keys and executes commands.
    class Server
    {
    public:
        /// Construct a server bound to `port` that enforces `api_key`.
        Server(uint16_t port, std::string api_key);
        ~Server();

        /// Initialize socket, bind, and start listening.
        /// @return true on success, false on failure
        bool start();
        /// Accept and handle connections (blocking, one at a time).
        void run();

    private:
        /// Handle a single client connection end-to-end.
        /// @param fd client socket descriptor
        void handle_client(int fd);
        /// Execute a shell command and capture exit code/stdout/stderr.
        /// @param command shell command to run
        /// @return response with exit code and captured output
        Response execute_command(const std::string &command);
        /// Read an entire fd into a string.
        /// @param fd file descriptor to read
        /// @return collected data
        static std::string read_fd_to_string(int fd);

        uint16_t port_;
        std::string api_key_;
        int listen_fd_;
    };

} // namespace txrx
