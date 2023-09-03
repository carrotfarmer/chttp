#pragma once

#include<iostream>

#include <ctime>

enum class RequestMethod {
    GET,
    POST,
    PATCH,
    PUT,
    DELETE,
};

namespace Utils {
    RequestMethod parse_request_method(std::string& header) {
        size_t i = header.find('\n');
        header = header.substr(0, i);
        size_t u = header.find(' ');
        header = header.substr(0, u);

        std::cout << "Incoming request method: " << header << std::endl;

        if (header == "GET") return RequestMethod::GET;
        else if (header == "POST") return RequestMethod::POST;
        else if (header == "PATCH") return RequestMethod::PATCH;
        else if (header == "PUT") return RequestMethod::PUT;
        else if (header == "DELETE") return RequestMethod::DELETE;
        else std::cerr << "ERR: Invalid request method!" << std::endl;
    }

    std::string gen_http_date() {
        char buf[1000];
        time_t now = time(nullptr);
        struct tm tm = *gmtime(&now);
        strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
        return buf;
    }
}