/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:47:59 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/23 16:58:23 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Constructors and destructors
Server::Server(void) : _socket(0), _client_socket(0) {}

Server::Server(const t_config &config) : _socket(0), _client_socket(0), _config(config) {}

Server::Server(const Server &src) {
    *this = src;
}

Server::~Server(void) {
    closeSocket();
}

// Operators
Server &Server::operator=(const Server &rhs) {
    if (this != &rhs) {
        this->_socket = rhs._socket;
        this->_client_socket = rhs._client_socket;
        this->_address = rhs._address;
        this->_client_address = rhs._client_address;
        this->_client_address_len = rhs._client_address_len;
    }
    return *this;
}

// Member functions
void Server::start(void) {
    std::cout << "Starting server..." << std::endl;

    int socket = Server::createSocket();
    Server::bindSocket(socket);

    Server::listenConnection(socket);
    std::cout << "Server is listening on port " << PORT << std::endl;
    while (true) {
        if (!Server::acceptConnection(socket))
            continue;

        int client_socket = this->getClientSocket();
        struct sockaddr_in client_address = this->getClientAddress();
        char *client_ip = inet_ntoa(client_address.sin_addr);
        uint16_t client_port = ntohs(client_address.sin_port);
        
        std::cout << std::endl;
        std::cout << "Connection accepted from " << client_ip << ":" << client_port << std::endl;
        std::cout << std::endl;

        Server::handleRequest(client_socket);
        
        close(client_socket);
    }
}

int Server::createSocket(void) {
    // Create a new socket file descriptor
    int socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1) {
        throw ServerException("Socket creation failed");        
    }

    // Store ou Return le socket... je sais pas
    this->setSocket(socket);
    return socket;
}

void Server::bindSocket(int socket) {
    // Initialize the address structure to zero before setting its parameters
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    // Set the family, port, and IP address in the sockaddr_in structure
    address.sin_family = AF_INET;            // Specifies the address family (IPv4)
    address.sin_port = ::htons(PORT);        // Specifies the port number, converted to network byte order
    address.sin_addr.s_addr = INADDR_ANY;    // Listen on all network interfaces

    // Attempt to bind the socket to the address and port specified
    int status = ::bind(socket, (struct sockaddr *)&address, sizeof(address));
    if (status == -1) {
        throw ServerException("Socket binding failed");
    }
}

void Server::listenConnection(int socket) {    
    // Attempt to listen on the socket for incoming connections
    int status = ::listen(socket, BACKLOG);
    if (status == -1) {
        throw ServerException("Socket listening failed");
    }
}

bool Server::acceptConnection(int socket) {
    // Initialize the address structure to zero before setting its parameters
    struct sockaddr_in client_address;
    memset(&client_address, 0, sizeof(client_address));
        
    // Accept a new connection on the socket and store the client's address in the client_address structure
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = ::accept(socket, (struct sockaddr *)&client_address, &client_address_len);
    
    // Check if the accept operation was successful
    if (client_socket == -1) {
        std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
        return false;
    }

    this->setClientSocket(client_socket);
    this->setClientAddress(client_address);
    this->setClientAddressLen(client_address_len);

    return true;
}


void Server::handleRequest(int client_socket) {
    // Initialize a buffer to store the incoming data
    char buffer[BUFFER_SIZE];
    
    // Receive data from the client and store it in the buffer
    ssize_t bytes_received = ::recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        throw ServerException("Failed to receive data");
    }
    
    // Print the received data
    std::cout << "Received " << bytes_received << " bytes: " << buffer << std::endl;
    
    // Send a response back to the client
    const char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello, World!";
    ssize_t bytes_sent = ::send(client_socket, response, strlen(response), 0);
    if (bytes_sent == -1) {
        throw ServerException("Failed to send data");
    }
    
    // Print the number of bytes sent
    std::cout << "Sent " << bytes_sent << " bytes: " << response << std::endl;
}

void Server::closeSocket(void) {
    // Retrieve the socket file descriptor
    int socket = this->getSocket();
    
    // Close the socket file descriptor
    ::close(socket);
}

// Getters
int Server::getSocket(void) const { return this->_socket; }

int Server::getClientSocket(void) const { return this->_client_socket; }

struct sockaddr_in Server::getAddress(void) const { return this->_address; }

struct sockaddr_in Server::getClientAddress(void) const { return this->_client_address; }

socklen_t Server::getClientAddressLen(void) const { return this->_client_address_len; }

// Setters

void Server::setSocket(int socket) { this->_socket = socket; }

void Server::setClientSocket(int client_socket) { this->_client_socket = client_socket; }

void Server::setAddress(struct sockaddr_in address) { this->_address = address; }

void Server::setClientAddress(struct sockaddr_in client_address) { this->_client_address = client_address; }

void Server::setClientAddressLen(socklen_t client_address_len) { this->_client_address_len = client_address_len; }