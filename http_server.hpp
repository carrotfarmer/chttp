#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "./utils.hpp"
#include "./http.hpp"
#include "./route.hpp"

class HttpServer {
public:
    explicit HttpServer(const std::string port) {
        if (port.find_first_not_of("0123456789") == std::string::npos) {
            m_port = port.c_str();
            return;
        };

        std::cerr << "ERROR: Don\'t be a dumbass. It\'s a port NUMBER." << std::endl;
    } // NOLINT(*-pro-type-member-init)

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

    void AddRoute(const std::string &path, const RequestMethod method, const std::string &content_type,
                  const std::string &content) {
        Route route = Route{path, method, content_type, content};
        m_routes.push_back(route);
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
            std::string p = Utils::parse_path(req);

            bool path_matched = false;

            for (const auto& route: m_routes) {
                if (route.request_method() == rm && route.path() == p) {
                    path_matched = true;
                    HttpResponse http_res = HttpResponse{200, route.content_type(), route.content()};
                    http_res.send_response(client);
                }
            }

            if (!path_matched) {
                HttpResponse http_res = HttpResponse{404, "text/plain", "404 Not Found"};
                http_res.send_response(client);
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
    std::vector<Route> m_routes;
    int m_socketfd;
};
