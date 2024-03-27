/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:03:36 by maroy             #+#    #+#             */
/*   Updated: 2024/03/27 16:09:18 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define BUFFER_SIZE 1024

#include "webserv.h"

class HTTPRequest;
class HTTPResponse;

class Client {
public:
	// Constructors & Destructors
	Client(int socket_fd);
	~Client(void);

	// Methods
	void readRequest(void);
	void writeResponse(void);
	bool hasPendingOperations(void) const;
	void disconnect(void);
	
	// Getters
	int getSocketFd(void) const;
	
private:
	// Attributes
	int _socket_fd;
	bool _is_writing;
	HTTPRequest *_request;
	HTTPResponse *_response;

	// Methods
	void processRequest(void);
};