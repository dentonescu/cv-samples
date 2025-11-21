#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>

#include "ansi.hpp"
#include "protocol.hpp"
#include "txrxcli.hpp"
#include "server/server.hpp"

namespace txrx
{

    Server::Server(uint16_t port, std::string api_key)
        : port_(port), api_key_(std::move(api_key)), listen_fd_(-1)
    {
    }

    Server::~Server()
    {
        if (listen_fd_ >= 0)
        {
            ::close(listen_fd_);
        }
    }

    bool Server::start()
    {
        listen_fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listen_fd_ < 0)
        {
            txrx::perror("socket");
            return false;
        }

        int opt = 1;
        // allow rebind to the socket to prevent "address already in use" on restart
        ::setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        // didactic note: hex value 0x1234 => typical host: 34 12 ; network: 12 34
        addr.sin_addr.s_addr = htonl(INADDR_ANY); // network byte order needs 32-bit big endian
        addr.sin_port = htons(port_);             // network byte order needs 16-bit big endian

        // bind expects sockaddr* and sockaddr_in* isn't implicitly convertible, hence reinterpret_cast
        if (::bind(listen_fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        {
            txrx::perror("bind");
            return false;
        }
        if (::listen(listen_fd_, MAX_PENDING_CONNECTIONS) < 0)
        {
            txrx::perror("listen");
            return false;
        }
        return true;
    }

    void Server::run()
    {
        txrx::log() << "Server listening on port: " << port_ << std::endl;

        for (;;)
        {
            int client_fd = ::accept(listen_fd_, nullptr, nullptr);
            if (client_fd < 0)
            {
                txrx::perror("accept");
                continue;
            }

            handle_client(client_fd); // one client at a time and some backlog; no async
            ::close(client_fd);
        }
    }

    void Server::handle_client(int fd)
    {
        auto parsed = decode_request_frame(fd);

        if (auto err = std::get_if<FrameError>(&parsed))
        {
            Response r{err->code, {}, err->message};
            send_buf_to_sock(fd, encode_response_frame(r));
            return;
        }

        const Request &req = std::get<Request>(parsed);
        if (req.api_key != api_key_)
        {
            Response resp1{kStatusUnauthorized, {}, std::string(kErrInvalidApiKey)};
            send_buf_to_sock(fd, encode_response_frame(resp1));
            return;
        }

        Response resp2 = execute_command(req.command);
        send_buf_to_sock(fd, encode_response_frame(resp2));
    }

    std::string Server::read_fd_to_string(int fd)
    {
        std::string out;
        char buffer[1024];
        while (true)
        {
            ssize_t got = ::read(fd, buffer, sizeof(buffer));
            if (got <= 0)
            {
                break;
            }
            out.append(buffer, static_cast<size_t>(got));
        }
        return out;
    }

    Response Server::execute_command(const std::string &command)
    {
        constexpr int kReader = 0;
        constexpr int kWriter = 1;
        constexpr int kExecFailureCode = 127;
        int out_pipe[2];
        int err_pipe[2];

        if (pipe(out_pipe) != 0) // out_pipe[0] can read bytes written to out_pipe[1]
        {
            return Response{kStatusInternalServerError, {}, "failed to create the one-way communication pipe for stdout"};
        }
        if (pipe(err_pipe) != 0) // err_pipe[0] is the read end, err_pipe[1] is the write end
        {
            ::close(out_pipe[kReader]);
            ::close(out_pipe[kWriter]);
            return Response{kStatusInternalServerError, {}, "failed to create the one-way communication pipe for stderr"};
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            ::close(out_pipe[kReader]);
            ::close(out_pipe[kWriter]);
            ::close(err_pipe[kReader]);
            ::close(err_pipe[kWriter]);
            return Response{kStatusInternalServerError, {}, "failed to fork the process"};
        }

        if (pid == 0)
        {
            // CHILD PROCESS (because the PID is zero)
            ::close(out_pipe[kReader]);
            ::close(err_pipe[kReader]);
            // redirect stdout to out_pipe[kWriter]
            ::dup2(out_pipe[kWriter], STDOUT_FILENO);
            // redirect stderr to err_pipe[kWriter]
            ::dup2(err_pipe[kWriter], STDERR_FILENO);
            ::close(out_pipe[kWriter]);
            ::close(err_pipe[kWriter]);
            execl("/bin/sh", "sh", "-c", command.c_str(), (char *)nullptr);
            // if execl returns, the new program did not replace the child
            _exit(kExecFailureCode);
        }

        // PARENT PROCESS (since PID is not zero)
        ::close(out_pipe[kWriter]);
        ::close(err_pipe[kWriter]);
        std::string stdout_text = read_fd_to_string(out_pipe[0]);
        std::string stderr_text = read_fd_to_string(err_pipe[0]);
        ::close(out_pipe[kReader]);
        ::close(err_pipe[kReader]);

        int status = 0;
        int exit_code = 0;
        if (waitpid(pid, &status, 0) >= 0)
        {
            if (WIFEXITED(status))
            {
                exit_code = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status))
            {
                // common Unix convention: when a process exits due to a signal,
                // we return 128 + signal_number.
                exit_code = 128 + WTERMSIG(status);
            }
            else
            {
                exit_code = EXIT_FAILURE;
            }
        }
        else
        {
            exit_code = EXIT_FAILURE;
        }

        return Response{exit_code, std::move(stdout_text), std::move(stderr_text)};
    }

} // namespace txrx
