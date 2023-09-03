#pragma once

#include <iostream>
#include <sstream>

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "./utils.hpp"
#include "./http.hpp"

class HttpServer {
public:
    explicit HttpServer(const char *port) : m_port(port) {}

    void Start() {
        int res_err = getaddrinfo("localhost", m_port, &m_hints, &m_res);
        if (res_err == -1) {
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
            std::cerr << "OOF SOMETHING AIN\'T RIGHT" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Running on port: " << m_port << std::endl;
        while (true) {
            int client = accept(m_socketfd, nullptr, nullptr);
            if (client == -1) {
                std::cerr << "Client connection failed!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::cout << "Client connection established." << std::endl;
            HandleConnection(client);
        }
    }

    ~HttpServer() {
        clean();
    }

    void Stop() {
        clean();
    }

private:

    void HandleConnection(int client) {
        char buffer[1024];
        std::string buf;
        std::string req;
        std::string header;

        ssize_t bytes_read = read(client, buffer, sizeof(buffer));

        if (bytes_read > 0) {
            buf.append(buffer, bytes_read);
            size_t header_end = buf.find("\r\n\r\n");

            if (header_end != std::string::npos) {
                req = buf.substr(0, header_end);
            }

            RequestMethod rm = Utils::parse_request_method(req);

            HttpResponse http_res = HttpResponse{ 200, "text/plain", "deez nuts" };
            auto res = http_res.generate_response();

            size_t res_len = res.length();
            std::cout << res.c_str() << std::endl;
            ssize_t bytes_sent = write(client, res.c_str(), res_len);

            if (bytes_sent == -1) {
                std::cerr << "Error sending response to client" << std::endl;
                std::cerr << strerror(errno) << std::endl;
            } else {
                std::cout << "Sent " << bytes_sent << " bytes to client" << std::endl;
            }

        } else {
            clean();
            std::cout << "Client closed connection." << std::endl;
        }
    }

    void clean() {
        freeaddrinfo(&m_hints);
        close(m_socketfd);
    }

    struct addrinfo m_hints{.ai_family = AF_UNSPEC, .ai_socktype=SOCK_STREAM}, *m_res;
    const char *m_port;
    int m_socketfd;
};
