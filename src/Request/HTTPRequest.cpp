/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 02:40:50 by maroy             #+#    #+#             */
/*   Updated: 2024/04/18 01:06:59 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "utils.h"
#include <fstream>
#include <sstream>

HTTPRequest::HTTPRequest(void) {
    this->_isHeaderEnd = false;
    this->_total = 0;
}

HTTPRequest::~HTTPRequest(void) {
    // this->clear();
}

void HTTPRequest::clear(void) {
    this->_method.clear();
    this->_uri.clear();
    // this->_headers.clear();
    this->_body.clear();
    this->_headerStr.clear();
    this->_isHeaderEnd = false;
    this->_total = 0;
}

std::string HTTPRequest::getMethod(void) {
    return (this->_headers[REQ_METHOD]);
}

std::string HTTPRequest::getURI(void) {
    return (this->_headers[REQ_PATH]);
}

std::string &HTTPRequest::getBody(void) {
    return (this->_body);
}

bool HTTPRequest::getHeaderEnd(void) const {
    return (this->_isHeaderEnd);
}

std::map<std::string, std::string> &HTTPRequest::getHeaders(void) {
    return (this->_headers);
}

void HTTPRequest::addToHeader(std::string key, std::string value) {
    this->_headers[key] = value;
}

std::string &HTTPRequest::getValueByKey(std::string key) {
    return (this->_headers[key]);
}

size_t HTTPRequest::getContentLenght(void) {
    return (std::atoll(this->_headers[REQ_CONTENT_LENGTH].c_str()));
}

void HTTPRequest::appendFile(char *buffer, int len) {
    this->_total += len;
    Logger::get().log(DEBUG, "Total: %d - Content-Length: %d", this->_total, this->getContentLenght());
    this->_body.append(buffer, len);
}

void HTTPRequest::appendHeader(char *buffer, int len) {
    std::string tmp;

    this->_headerStr.append(buffer, len);
    size_t index = this->_headerStr.find("\r\n\r\n");
    if (index != std::string::npos) {
        this->_isHeaderEnd = true;
        this->_body = this->_headerStr.substr(index + 4, this->_headerStr.length() - index);
        this->_total += this->_body.length();
        this->_headerStr = this->_headerStr.substr(0, index + 4);

        check_request(this->_headerStr);
        std::vector<std::string> lines = split_line(this->_headerStr, "\r\n");
        if (lines.size() > 0)
            parse_start_line(this->_headers, lines[0]);
        parse_header(this->_headers, lines);
        check_path(this->_headers[REQ_PATH], this->_headers[REQ_CONNECTION]);
        if (this->_headers[REQ_METHOD] == "POST") {
            bool content_lenght = !this->_headers[REQ_CONTENT_LENGTH].empty();
            bool transfer = !this->_headers[REQ_TRANSFER].empty();
            if (!content_lenght && !transfer)
                throw(BAD_REQUEST_STATUS);
            if (content_lenght && transfer)
                throw(BAD_REQUEST_STATUS);
            if (transfer && this->_headers[REQ_TRANSFER] != "chunked")
                throw(NOT_IMPLEMENTED_STATUS);
        }
    }
}
