#include <iostream>

#include <sys/socket.h>
#include <netdb.h>

const char* PORT = "4200";

int main() {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // AF_UNIX -> Internet Domain
    // SOCK_STREAM -> TCP
    // O -> Internet Protocol

    int res_err = getaddrinfo("localhost", PORT, &hints, &res);
    if (res_err == -1)  {
        std::cerr << "getaddrinfo failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (res == nullptr) {
        std::cerr << "getaddrinfo failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    const int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int err = bind(socket_fd, res->ai_addr, res->ai_addrlen);

    if (err == -1) {
        std::cerr << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, SOMAXCONN) == -1) {
        std::cerr << "OOF SOMETHING AINT RIGHT" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Running on port " << PORT << std::endl;
    while (true) {
        int client = accept(socket_fd, NULL, NULL);
        if (client == -1) {
            std::cerr << "Client connection failed!" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Client connection established." << std::endl;
    }
}
