/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:58:56 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/11 16:00:59 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.h"
#include "Request/HTTPRequest.hpp"

class HTTPRequest;

class CGIHandler {
public:
    CGIHandler(HTTPRequest *request, t_server &server, std::string _path);
    ~CGIHandler(void);

    std::string executeCGI(t_location &location);

private:
    void setupEnvironment(void);

    HTTPRequest *_request;
    t_server _server;
    std::string _path;
};
