/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:58:53 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/11 16:00:48 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

extern char **environ;

const std::string getExtension(const std::string &path);

CGIHandler::CGIHandler(HTTPRequest *request, t_server &server, std::string path)
    : _request(request), _server(server), _path(path) {
    (void)_request;
    return;
}
CGIHandler::~CGIHandler(void) {
    return;
}

std::string CGIHandler::executeCGI(t_location &location) {

    std::string cgi_ext;
    std::string cgi_exec;

    cgi_ext = getExtension(this->_path);
    if (!this->_server.cgi.empty()) {
        cgi_exec = this->_server.cgi[cgi_ext];
    } else {
        cgi_exec = location.cgi[cgi_ext];
    }

    (void)this->_server;
    int pipefd[2];
    pipe(pipefd);

    std::cout << "Executing CGI script: " << this->_path << std::endl;
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return "Internal Server Error";
    } else if (pid == 0) {
        // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Setup arguments and environment
        const char *interpreter = cgi_exec.c_str();  // Or "/usr/bin/env python3"
        char *const argv[] = {const_cast<char *>(interpreter), const_cast<char *>(this->_path.c_str()), NULL};

        setupEnvironment();

        execve(interpreter, argv, environ);

        perror("execve failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(pipefd[1]);
        std::string output;
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.append(buffer, bytesRead);
        }
        close(pipefd[0]);

        waitpid(pid, NULL, 0);

        return output;
    }
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
}

// extension
// method
// reponse ?
// query string
