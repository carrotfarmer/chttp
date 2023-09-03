#pragma once

#include <iostream>

#include "./utils.hpp"

struct Route {
public:
    Route(std::string path, RequestMethod method, std::string content_type, std::string content) : m_path(
            std::move(path)), m_request_method(method), m_content_type(std::move(content_type)), m_content(
            std::move(content)) {};

    [[nodiscard]] RequestMethod request_method() const noexcept {
       return m_request_method;
    }

    [[nodiscard]] std::string path() const noexcept {
        return m_path;
    }

    [[nodiscard]] std::string content_type() const noexcept {
        return m_content_type;
    }

    [[nodiscard]] std::string content() const noexcept {
        return m_content;
    }

private:
    std::string m_path;
    RequestMethod m_request_method;
    std::string m_content_type;
    std::string m_content;
};