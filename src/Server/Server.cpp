#include "../../include/Server.hpp"

// Constructors
Server::Server(t_config config): _config(config){
}

Server::Server(void){
}

Server::Server(Server const &src){
	*this = src;
}

// Destructor
Server::~Server(void){

    freeaddrinfo(this->_addressInfo); // Free memory
}

// Operators
Server	&Server::operator=(Server const &rhs){
	(void)rhs;
	return (*this);
}

// Member functions
int Server::convertAddressToIP(const std::string &address) {
    addrinfo hints; // Hints or "filters" for getaddrinfo()
    addrinfo *r;    // Pointer to iterate on results
    int status; // Return value of getaddrinfo()
    char buffer[INET6_ADDRSTRLEN]; // Buffer to convert IP address

    memset(&hints, 0, sizeof hints); // Initialize the structure
    hints.ai_family = AF_INET; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Get the associated IP address(es)
    status = getaddrinfo(address.c_str(), "80", &hints, &this->_addressInfo);
    if (status != 0) { // error !
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return (2);
    }

    std::cout << "IP adresses for " << address << ":" << std::endl;

    r = this->_addressInfo; // First address in getaddrinfo()'s results
    while (r != NULL) {
        // we need to cast the address as a sockaddr_in structure to
        // get the IP address, since ai_addr might be either
        // sockaddr_in (IPv4) or sockaddr_in6 (IPv6)
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)r->ai_addr;
        // Convert the integer into a legible IP address string
        inet_ntop(r->ai_family, &(ipv4->sin_addr), buffer, sizeof buffer);
        std::cout << "IPv4: " << buffer << std::endl;
        r = r->ai_next; // Next address in getaddrinfo()'s results
    }
    return (0);
}

int Server::prepareSocket(void) {
    addrinfo hints; // Hints or "filters" for getaddrinfo()

    memset(&hints, 0, sizeof hints); // Initialize the structure
    hints.ai_family = AF_INET; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // Use my IP

    // Get the associated IP address(es)
    int status = getaddrinfo(NULL, PORT, &hints, &this->_addressInfo);
    if (status != 0) { // error !
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return (1);
    }

    // create socket, bind it and listen with it
    int sockfd = socket(this->_addressInfo->ai_family, this->_addressInfo->ai_socktype, this->_addressInfo->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Allow the socket to be reused
    status = bind(sockfd, this->_addressInfo->ai_addr, this->_addressInfo->ai_addrlen);
    if (status == -1) {
        perror("bind");
        exit(1);
    }

    // 
    status = listen(sockfd, SOMAXCONN); // SOMAXCONN
    if (status == -1) {
        perror("listen");
        exit(1);
    }

    std:: cout << "Server is listening on port " << PORT << std::endl;

    // Accept incoming connection


    sockaddr_storage clientAddress;
    socklen_t addr_size = sizeof(clientAddress);

    char buffer[1026]; // Buffer to convert IP address

    while (true) {
        int clientfd = accept(sockfd, (struct sockaddr *)&clientAddress, &addr_size);
        if (clientfd == -1) {
            perror("accept");
            exit(1);
        }

        write(clientfd, "Hello, world!", 13);

        read(clientfd, buffer, 1024);

        std::cout << "Server: got message: " << buffer << std::endl;

        std::cout << "Server: got connection from " << inet_ntoa(((struct sockaddr_in *)&clientAddress)->sin_addr) << std::endl;

        close(clientfd);
    }



    // Close the socket
    close(sockfd);
    
    return (0);
}

// int Server::prepareSocket(void) {
//     int socketfd; // File descriptor for the socket
//     int status;

//     socketfd = socket(this->_addressInfo->ai_family, this->_addressInfo->ai_socktype, this->_addressInfo->ai_protocol);

//     if (socketfd == -1) {
//         perror("socket");
//         exit(1);
//     }

//     status = bind(socketfd, this->_addressInfo->ai_addr, this->_addressInfo->ai_addrlen);

//     std::cout << "Socket fd: " << socketfd << std::endl;
//     std::cout << "Address: " << this->_addressInfo->ai_addr << std::endl;
//     std::cout << "Address length: " << this->_addressInfo->ai_addrlen << std::endl;

//     if (status == -1) {
//         perror("bind");
//         exit(1);
//     }

//     status = listen(socketfd, 2);

//     close(socketfd);

//     return (socketfd);
// }

// int Server::prepareSocket(void) {
//     int sockfd; // File descriptor for the socket
//     int yes = 1; // For setsockopt()
//     addrinfo *p; // Pointer to iterate on results

//     // Iterate through all the results and bind to the first we can
//     for(p = this->_addressInfo; p != NULL; p = p->ai_next) {
//         // Create the socket
//         sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
//         if (sockfd == -1) {
//             perror("server: socket");
//             continue;
//         }

//         // Allow the socket to be reused
//         if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
//             perror("setsockopt");
//             exit(1);
//         }

//         // Bind the socket to the port
//         if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
//             close(sockfd);
//             perror("server: bind");
//             continue;
//         }

//         break;
//     }

//     freeaddrinfo(this->_addressInfo); // Free memory

//     if (p == NULL) {
//         std::cerr << "server: failed to bind" << std::endl;
//         exit(1);
//     }

//     return (sockfd);
// }

