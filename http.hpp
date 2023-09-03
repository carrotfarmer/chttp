#pragma once

#include <iostream>
#include <sstream>

#include "./utils.hpp"

class HttpResponse {
public:
    HttpResponse(unsigned short status_code, std::string content_type, std::string content) : m_status(status_code),
                                                                         m_content_type(std::move(content_type)),
                                                                         m_content(std::move(content)) {};

    void send_response(int client) {
        std::stringstream res_stream;
        // status line
        res_stream << "HTTP/1.1 " << m_status << " OK" << "\n";
        res_stream << "Date: " << Utils::gen_http_date() << " GMT" << "\n";
        res_stream << "Content-Length: " << m_content.length() << "\n";
        res_stream << "Content-Type: " << m_content_type << "\r\n\r\n" << m_content;

        auto res = res_stream.str();

        size_t res_len = res.length();
        std::cout << res.c_str() << std::endl;
        ssize_t bytes_sent = write(client, res.c_str(), res_len);

        if (bytes_sent == -1) {
            std::cerr << "Error sending response to client" << std::endl;
            std::cerr << strerror(errno) << std::endl;
        } else {
            std::cout << "Sent " << bytes_sent << " bytes to client" << std::endl;
        }
    }

private:
    unsigned short m_status;
    std::string m_content_type;
    std::string m_content;
};