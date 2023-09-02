#pragma once

#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

class HttpServer {
public:
    explicit HttpServer(const char* port) : m_port(port) {}

    void Start() {
        int res_err = getaddrinfo("localhost", m_port, &m_hints, &m_res);
        if (res_err == -1)  {
            std::cerr << "getaddrinfo failed!" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (m_res == nullptr) {
            std::cerr << "getaddrinfo failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        m_socketfd = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol);

        int err = bind(m_socketfd, m_res->ai_addr, m_res->ai_addrlen);

        if (err == -1) {
            std::cerr << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        if (listen(m_socketfd, SOMAXCONN) == -1) {
            std::cerr << "OOF SOMETHING AINT RIGHT" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Running on port " << m_port << std::endl;
        while (true) {
            int client = accept(m_socketfd, nullptr, nullptr);
            if (client == -1) {
                std::cerr << "Client connection failed!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::cout << "Client connection established." << std::endl;
        }
    }

    ~HttpServer() {
        freeaddrinfo(&m_hints);
        close(m_socketfd);
    }

    void Stop() {
        freeaddrinfo(&m_hints);
        close(m_socketfd);
    }

private:
    struct addrinfo m_hints{.ai_family = AF_UNSPEC, .ai_socktype=SOCK_STREAM}, *m_res;
    const char* m_port;
    int m_socketfd;
};
