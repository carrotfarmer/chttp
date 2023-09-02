#include <csignal>

#include "http_server.hpp"

const char* PORT = "4200";

HttpServer* server_ptr = nullptr;

int main() {
    HttpServer server = HttpServer{PORT};
    server.Start();

    server_ptr = &server;

    auto handle_termination = [](int exit_code)
    {
        server_ptr->Stop();
        std::cout << "Exit detected. Cleaned up!" << std::endl;
        exit(exit_code);
    };

    signal(SIGTERM, handle_termination);
    signal(SIGINT, handle_termination);

    return 0;
}
