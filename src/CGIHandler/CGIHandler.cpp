/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:58:53 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/16 23:23:58 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include <sys/wait.h>

extern char **environ;

const std::string getExtension(const std::string &path);

CGIHandler::CGIHandler(HTTPRequest *request, t_server &server, std::string path)
    : _request(request), _server(server), _path(path) {
    (void)_request;

    this->setupEnvironment();
    return;
}
CGIHandler::~CGIHandler(void) {
    return;
}

void CGIHandler::setupEnvironment(void) {
    // Convert content length to string
    size_t contentLength = this->_request->getContentLenght();
    int port = this->_server.port;
    std::string portStr = std::to_string(port);
    std::string contentLengthStr = std::to_string(contentLength);

    setenv("SERVER_SOFTWARE", this->_server.server_name.c_str(), true);
    setenv("SERVER_NAME", this->_server.ip_address.c_str(), true);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", true);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", true);
    setenv("SERVER_PORT", portStr.c_str(), true);
    setenv("REQUEST_METHOD", this->_request->getMethod().c_str(), true);
    setenv("PATH_INFO", this->_request->getURI().c_str(), true);
    setenv("PATH_TRANSLATED", this->_path.c_str(), true);
    setenv("SCRIPT_NAME", this->_path.c_str(), true);
    setenv("QUERY_STRING", this->_request->getValueByKey(REQ_QUERY_STRING).c_str(), true);
    setenv("REMOTE_ADDR", this->_server.ip_address.c_str(), true);
    setenv("AUTH_TYPE", "", true);
    setenv("REMOTE_USER", "", true);
    setenv("REMOTE_IDENT", "", true);
    setenv("CONTENT_TYPE", "", true);
    setenv("CONTENT_LENGTH", contentLengthStr.c_str(), true);

    this->_env = environ;
    std::cout << "Setting up environment for CGI: Done" << std::endl;
}

char **CGIHandler::getEnv(void) const {
    return this->_env;
}

int CGIHandler::getFdIn(void) const {
    return this->_fd_in;
}

int CGIHandler::getFdOut(void) const {
    return this->_fd_out;
}

pid_t CGIHandler::getPid(void) const {
    return this->_pid;
}

void CGIHandler::setFdIn(int fd) {
    this->_fd_in = fd;
}

void CGIHandler::setFdOut(int fd) {
    this->_fd_out = fd;
}

void CGIHandler::setPid(pid_t pid) {
    this->_pid = pid;
}

// extension
// method
// reponse ?
// query string
