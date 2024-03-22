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
    status = getaddrinfo(address.c_str(), 0, &hints, &this->_addressInfo);
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
    int socketfd; // File descriptor for the socket

    socketfd = socket(this->_addressInfo->ai_family, this->_addressInfo->ai_socktype, this->_addressInfo->ai_protocol);
    std::cout << "Socket file descriptor: " << socketfd << std::endl;

    std::cout << connect(socketfd, this->_addressInfo->ai_addr, this->_addressInfo->ai_addrlen) << std::endl;


    close(socketfd);

    return (socketfd);
}

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

