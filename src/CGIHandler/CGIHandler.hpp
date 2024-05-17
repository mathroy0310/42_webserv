/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:58:56 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/16 23:23:46 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request/HTTPRequest.hpp"
#include "defines.h"

class HTTPRequest;

class CGIHandler {
  public:
    CGIHandler(HTTPRequest *request, t_server &server, std::string _path);
    ~CGIHandler(void);

    char **getEnv(void) const;
    int getFdIn(void) const;
    int getFdOut(void) const;
	pid_t getPid(void) const;

    void setFdIn(int fd);
	void setFdOut(int fd);
	void setPid(pid_t pid);

  private:
    void setupEnvironment(void);
    HTTPRequest *_request;
    char **_env;
    t_server _server;
    std::string _path;
	pid_t _pid;
    int _fd_in;
    int _fd_out;
};
