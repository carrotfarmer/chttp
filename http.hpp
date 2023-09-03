#pragma once

#include <iostream>
#include <sstream>

#include "./utils.hpp"

class HttpResponse {
public:
    HttpResponse(unsigned short status_code, std::string content_type, std::string content) : m_status(status_code),
                                                                         m_content_type(std::move(content_type)),
                                                                         m_content(std::move(content)) {};

    std::string generate_response() {
        std::stringstream res;
        // status line
        res << "HTTP/1.1 " << m_status << " OK" << "\n";
        res << "Date: " << Utils::gen_http_date() << " GMT" << "\n";
        res << "Content-Length: " << m_content.length() << "\n";
        res << "Content-Type: " << m_content_type << "\r\n\r\n" << m_content;

        return res.str();
    }

private:
    unsigned short m_status;
    std::string m_content_type;
    std::string m_content;
};