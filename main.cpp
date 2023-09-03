#include <csignal>

#include "http_server.hpp"
#include "template.hpp"

const std::string PORT = "4200";

HttpServer *server_ptr = nullptr;

int main() {
    HttpServer server = HttpServer{PORT};

    // NOTE: bin is located in cmake-build-debug/ so move up one dir and access the templates dir
    Template awesome_template = Template{"../templates/html.html"};

    server.AddRoute("/", RequestMethod::GET, "text/plain", "deez nuts");
    server.AddRoute("/deez", RequestMethod::GET, "text/plain", "nuts");
    server.AddRoute("/html", RequestMethod::GET, "text/html", awesome_template.html());

    server.Start();

    server_ptr = &server;

    auto handle_termination = [](int exit_code) {
        server_ptr->Stop();
        std::cout << "Exit detected. Cleaned up!" << std::endl;
        exit(exit_code);
    };

    signal(SIGTERM, handle_termination);
    signal(SIGINT, handle_termination);

    return 0;
}
