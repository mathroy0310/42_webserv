/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:04:36 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/27 18:47:39 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/HTTPResponse.hpp"

HTTPResponse::HTTPResponse(void) {}

HTTPResponse::~HTTPResponse(void) {}

std::string HTTPResponse::serialize(void) const {
    std::string response;

    response += this->_version + " " + this->_status_code + " " + this->_status_message + "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n";
    response += this->_body;
    return (response);
}

void HTTPResponse::setVersion(const std::string &version) {
    this->_version = version;
}

void HTTPResponse::setStatusCode(const std::string &status_code) {
    this->_status_code = status_code;
}

void HTTPResponse::setStatusMessage(const std::string &status_status_message) {
    this->_status_message = status_status_message;
}

void HTTPResponse::setHeaders(const std::map<std::string, std::string> &headers) {
    this->_headers = headers;
}

void HTTPResponse::setBody(const std::string &body) {
    this->_body = body;
}

std::string HTTPResponse::getVersion(void) const {
    return (this->_version);
}

std::string HTTPResponse::getStatusCode(void) const {
    return (this->_status_code);
}

std::string HTTPResponse::getStatusMessage(void) const {
    return (this->_status_message);
}

std::map<std::string, std::string> HTTPResponse::getHeaders(void) const {
    return (this->_headers);
}

std::string HTTPResponse::getBody(void) const {
    return (this->_body);
}
