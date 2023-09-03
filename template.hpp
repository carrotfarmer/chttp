#pragma once

#include <iostream>
#include <fstream>

class Template {
public:
    explicit Template(std::string template_path) : m_template_path(std::move(template_path)) {};

    std::string html() {
        std::ifstream ifs(m_template_path);
        std::string content((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));
        return content;
    }

private:
    const std::string m_template_path;
};