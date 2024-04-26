	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:58:53 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/18 01:48:36 by maroy            ###   ########.fr       */
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
    for (int i = 0; _env[i] != NULL; i++) {
        free(_env[i]);
    }
    delete[] _env;
    return;
}

static void convertToUpperCaseUnderscore(std::string &input) {
    std::string result;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '-') {
            input[i] = '_';
        } else if (input[i] != '_' && std::isalpha(input[i]) && std::islower(input[i])) {
            input[i] -= 32;
        }
    }
}

void CGIHandler::setupEnvironment(void) {
    char **env;
    std::map<std::string, std::string> &tmp = this->_request->getHeaders();
    std::map<std::string, std::string>::iterator it;
    int i;

	i = 0;

	env = new char*[tmp.size() + 1];
	for (it = tmp.begin(); it != tmp.end(); it++){
		std::string key = it->first;
        convertToUpperCaseUnderscore(key);
        env[i++] = strdup((key + "=" + it->second).c_str());
		std::cout << env[i - 1] << std::endl;
    }
    env[i] = NULL;
	this->_env = env;
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
