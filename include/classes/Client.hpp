/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:03:36 by maroy             #+#    #+#             */
/*   Updated: 2024/03/26 00:39:23 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define BUFFER_SIZE 1024

#include "webserv.h"

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
	std::string _request_buffer;
	std::string _response_buffer;
	bool _is_writing;

	// Methods
	// void parseRequest(void);
	void handleRequest(void);
};