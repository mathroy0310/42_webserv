/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 02:21:16 by maroy             #+#    #+#             */
/*   Updated: 2024/04/08 20:37:20 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "utils.h"

void set_key_and_value(std::string line, std::map<std::string, std::string> &headermap) {
    std::string key, value;

    std::istringstream iss(line);
    char delim;
    if (line.find(":") != std::string::npos)
        delim = ':';
    else
        throw(BAD_REQUEST_STATUS);
    std::getline(iss, key, delim);
    key = trim(key);
    value = &line[key.length() + 2];
    if (key.empty() || value.empty())
        return;

    if (value[0] == '\"' && value[value.length() - 1] == '"')
        value = value.substr(1, value.length() - 2);
    if (key == "Host" && !headermap[key].empty())
        throw(BAD_REQUEST_STATUS);
    headermap[key] = value;
    if (key == "Host") {
        value = headermap[key];
        headermap.erase(key);
        key = REQ_SERVER_NAME;
        std::istringstream iss(value);
        std::string servername, port;
        std::getline(iss, servername, delim);
        std::getline(iss, port, delim);
        headermap[REQ_SERVER_NAME] = servername;
        if (!port.empty())
            headermap[REQ_PORT] = port;
    } else
        headermap[key] = value;
}

void parse_header(std::map<std::string, std::string> &headermap, std::vector<std::string> lines) {
    // std::string key;
    // std::string value;

    for (size_t i = 1; i < lines.size(); i++) {
        set_key_and_value(lines[i], headermap);
        // if (lines[i].find(";") != std::string::npos && lines[i].find(REQ_BOUNDARY) == std::string::npos)
        // 	valuesWithinLine = splitLine(lines[i], ";");
        // else
        // 	valuesWithinLine.push_back(lines[i]);
        // for (size_t j = 0; j < valuesWithinLine.size(); j++) {
        // 	try {
        // 	} catch(std::exception &e) {
        // 		e.what();
        // 	}
        // }
    }
}

void parse_start_line(std::map<std::string, std::string> &headermap, std::string line) {
    std::vector<std::string> first_line;
    std::vector<std::string> splitted_line;

    std::istringstream iss(line);
    while (std::getline(iss, line, ' '))
        if (!line.empty())
            first_line.push_back(line);
    if (first_line.size() == 3) {
        headermap[REQ_METHOD] = trim(first_line[0]);
        if (first_line[1].find("?") != std::string::npos) {
            splitted_line = split_line(first_line[1], "?");
            headermap[REQ_PATH] = splitted_line[0];
            if (splitted_line.size() > 1)
                headermap[REQ_QUERY_STRING] = splitted_line[1];
        } else
            headermap[REQ_PATH] = trim(first_line[1]);
        headermap[REQ_HTTP_VERSION] = trim(first_line[2]);
        if (headermap[REQ_HTTP_VERSION] != "HTTP/1.1")
            throw(HTTP_VERSION_NOT_SUPPORTED_STATUS);

    } else
        throw(BAD_REQUEST_STATUS);
}

static void remove_spaces(std::string &buffer, size_t index) {
    size_t start;

    start = index;
    while (start < buffer.length() && buffer[start] == ' ')
        start++;
    if (start > index + 1)
        buffer.erase(index, start - index - 1);
}

static void check_enclosed(char c, std::stack<char> &stack) {
    if (stack.empty() || stack.top() != c)
        stack.push(c);
    else
        stack.pop();
}

static void check_line(std::string &line, std::stack<char> stack, bool &is_quotes) {
    for (size_t j = 0; j < line.size(); j++) {
        if (line[j] == '\"' || line[j] == '\'') {
            is_quotes = !is_quotes;
            check_enclosed(line[j], stack);
        } else if (line[j] == ' ' && !is_quotes)
            remove_spaces(line, j);
    }
}

void check_request(std::string buffer) {
    std::stack<char> stack;
    std::string head;
    bool is_quotes;

    is_quotes = false;
    size_t last = buffer.size() - 1;
    if (buffer[last] != '\n' || buffer[last - 1] != '\r')
        throw(BAD_REQUEST_STATUS);
    std::vector<std::string> lines = split_line(buffer, "\r\n");
    for (size_t i = 0; i < lines.size(); i++) {
        check_line(lines[i], stack, is_quotes);
    }
    if (stack.size())
        throw(BAD_REQUEST_STATUS);
}

void check_path(std::string path, std::string connection) {
    if (path.length() > 2048) {
        if (connection == "keep-alive")
            throw(1);
        throw(0);
    }
    if (path.find_first_not_of(ALLOWED_URI_CHARS) != std::string::npos)
        throw(BAD_REQUEST_STATUS);
}
