#include "HTTPRequest.hpp"
#include <fstream>
#include <sstream>

HTTPRequest::HTTPRequest(void) {}

HTTPRequest::~HTTPRequest(void) {
    this->clear();
}

HTTPRequest &HTTPRequest::operator=(HTTPRequest const &rhs) {
    this->_method = rhs._method;
    this->_uri = rhs._uri;
    this->_version = rhs._version;
    this->_headers = rhs._headers;
    this->_body = rhs._body;
    return (*this);
}

void HTTPRequest::parse(const std::string &raw_request) {
    std::istringstream stream(raw_request);
    std::string line;

    this->clear();
    while (std::getline(stream, line)) {
        if (line.empty()) {
            break;
        }
        if (this->_method.empty()) {
            this->parseRequestLine(line);
        } else {
            this->parseHeaders(line);
        }
    }
    this->parseBody(stream.str());
}

void HTTPRequest::clear(void) {
    this->_method.clear();
    this->_uri.clear();
    this->_version.clear();
    this->_headers.clear();
    this->_body.clear();
}

std::string HTTPRequest::getMethod(void) const {
    return (this->_method);
}

std::string HTTPRequest::getURI(void) const {
    return (this->_uri);
}

std::string HTTPRequest::getVersion(void) const {
    return (this->_version);
}

std::map<std::string, std::string> HTTPRequest::getHeaders(void) const {
    return (this->_headers);
}

std::string HTTPRequest::getBody(void) const {
    return (this->_body);
}

void HTTPRequest::parseRequestLine(const std::string &line) {
    std::istringstream stream(line);
    std::string token;

    stream >> this->_method;
    stream >> this->_uri;
    stream >> this->_version;
}

void HTTPRequest::parseHeaders(const std::string &line) {
    size_t pos = line.find(':');
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 2);

    this->_headers[key] = value;
}

void HTTPRequest::parseBody(const std::string &line) {
    this->_body = line;
}
