#pragma once

#define BUFFER_SIZE 1024

#include "defines.h"
#include "parsing.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTTPRequest;
class HTTPResponse;

class Client {
public:
	// Constructors & Destructors
	Client(int socket_fd, t_server server);
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
	t_server _server;
	bool _is_writing;
	HTTPRequest *_request;
	HTTPResponse *_response;

	// Methods
	void processRequest(void);
	std::string getFilePath(void);
	void serveFile(const std::string &file_path, std::map<std::string, std::string> &headers);
	std::string getMimeType(const std::string &file_path);
};
