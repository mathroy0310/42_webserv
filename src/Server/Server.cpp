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
}

// Operators
Server	&Server::operator=(Server const &rhs){
	(void)rhs;
	return (*this);
}

// Member functions
int Server::convertIPToInt(const std::string &Address) {
    struct addrinfo hints; // Hints or "filters" for getaddrinfo()
    struct addrinfo *res;  // Result of getaddrinfo()
    struct addrinfo *r;    // Pointer to iterate on results
    int status; // Return value of getaddrinfo()
    char buffer[INET6_ADDRSTRLEN]; // Buffer to convert IP address

    memset(&hints, 0, sizeof hints); // Initialize the structure
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Get the associated IP address(es)
    status = getaddrinfo(Address, 0, &hints, &res);
    if (status != 0) { // error !
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return (2);
    }

    std::cout << "IP adresses for " << Address << ":" << std::endl;

    r = res;
    while (r != NULL) {
        void *addr; // Pointer to IP address
        // we need to cast the address as a sockaddr_in structure to
        // get the IP address, since ai_addr might be either
        // sockaddr_in (IPv4) or sockaddr_in6 (IPv6)
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)r->ai_addr;
        // Convert the integer into a legible IP address string
        inet_ntop(r->ai_family, &(ipv4->sin_addr), buffer, sizeof buffer);
        std::cout << "IPv4: " << buffer << std::endl;
        r = r->ai_next; // Next address in getaddrinfo()'s results
    }
    freeaddrinfo(res); // Free memory
    return (0);
}

