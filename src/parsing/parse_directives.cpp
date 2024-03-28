/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_directives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 20:48:46 by maroy             #+#    #+#             */
/*   Updated: 2024/03/28 15:10:56 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

std::string set_index(std::string &value, const std::string &key) {
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    return (value);
}

bool set_autoindex(std::string &value, const std::string &key) {
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    if (value == "0" || value == "false" || value == "FALSE" || value == "off" || value == "OFF")
        return (false);
    else if (value == "1" || value == "true" || value == "TRUE" || value == "on" || value == "ON")
        return (true);
    else {
        std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
}

std::string set_root(std::string &value, const std::string &key) {
    value = trim(value);
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    return (value);
}

std::string set_server_name(std::string &value, const std::string &key) {
    value = trim(value);
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    return (value);
}

static std::string set_ip_address(std::string &value, const std::string &key) {
    value = trim(value);
    if (value == "localhost")
        return ("127.0.0.1");
    if (value.find_first_not_of("0123456789.") != value.npos || std::count(value.begin(), value.end(), '.') != 3) {
        std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    std::istringstream iss(value);
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (token.empty() || token.find_first_not_of("0123456789") != token.npos || std::stoi(token) < 0 ||
            std::stoi(token) > 255) {
            std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
            exit(EXIT_FAILURE);
        }
    }
    return (value);
}

static int set_port_number(std::string &value, const std::string &key) {
    int port;

    value = trim(value);
    if (value.find_first_not_of("0123456789") != value.npos) {
        std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    port = std::stoi(value);
    if (port < 0 || port > 65535) {
        std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    return (port);
}

int set_port_and_ip_address(std::string &value, const std::string &key, std::string &ip_address) {
    size_t separator;

    value = trim(value);
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    separator = value.find(":");
    if (separator != value.npos) {
        std::string ip = value.substr(0, separator);
        std::string port = value.substr(separator + 1, -1);
        if (ip.empty() || port.empty()) {
            std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
            exit(EXIT_FAILURE);
        }
        ip_address = set_ip_address(ip, key);
        return (set_port_number(port, key));
    } else {
        if (value == "localhost") {
            ip_address = "127.0.0.1";
            return (8080);
        }
        if (value.find('.') != value.npos) {
            ip_address = set_ip_address(value, key);
            return (8080);
        } else if (value.find_first_not_of("0123456789") != value.npos) {
            ip_address = "0.0.0.0";
            return (set_port_number(value, key));
        } else {
            std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
            exit(EXIT_FAILURE);
        }
    }
}

void set_error_pages(std::string &value, const std::string &key, std::map<unsigned int, std::string> &error_pages) {
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    std::string token;
    std::istringstream em(value);
    while (std::getline(em, token)) {
        token = trim(token);
        size_t first = token.find_first_of(",");
        if (first == std::string::npos) {
            std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
            exit(EXIT_FAILURE);
        }
        std::string error_index = trim(token.substr(0, first));
        std::string error_value = trim(token.substr(first + 1, -1));
        if (error_index == "" || error_value == "" || error_index.find_first_not_of("0123456789") != value.npos) {
            std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
            exit(EXIT_FAILURE);
        }
        // std::cout << DEBUG_PREFIX << key << ":: Index : " << error_index;
        // std::cout << " | Value : " << error_value << std::endl;
        error_pages[std::stoi(error_index.c_str())] = error_value;
    }
}

size_t set_max_client_size(std::string &value, const std::string &key) {
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    if (value.find_first_not_of("0123456789") != value.npos) {
        std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    size_t max_client_size = std::stoi(value);
    return (max_client_size);
}
