#include "Server.hpp"

// Constructors and destructors

Server::Server(const t_config &config) : _config(config), _multiplexer(new Multiplexer()), _running(false) {}

Server::~Server(void) {
    this->stop();
}

void Server::run(void) {
    Logger::get().log(INFO, "Server starting...");
    try {
        this->_running = true;
        this->setupServerConnections();
        Logger::get().log(INFO, "Server connections setting up...");
        while (this->_running)
            if (this->_multiplexer->wait() > 0) {
                this->acceptConnections();
                this->handleRequests();
            }
    } catch (const std::exception &e) {
        Logger::get().log(ERROR, "Server error: %s", e.what());
        this->stop();
    }
}

void Server::stop(void) {
    if (this->_running) {
        this->_running = false;
        Logger::get().log(INFO, "Server stopping...");

        std::map<int, SocketWrapper>::iterator server = this->_listening_sockets.begin();
        for (; server != this->_listening_sockets.end(); server++) {
            close(server->first);
        }
        std::vector<Client>::iterator client = this->_clients.begin();
        for (; client != this->_clients.end(); client++) {
            client->disconnect();
        }
        delete _multiplexer;
    }
}

void Server::setupServerConnections(void) {
    for (size_t i = 0; i < this->_config.servers.size(); i++) {
        std::string ip_address = this->_config.servers[i].ip_address;
        int port = this->_config.servers[i].port;

        SocketWrapper new_socket(ip_address, port);
        new_socket.init();
        this->_multiplexer->addFd(new_socket.getSocketFd(), POLLIN);
        this->_listening_sockets.insert(std::make_pair(new_socket.getSocketFd(), new_socket));
        Logger::get().log(DEBUG, "Server socket set up on address %s and port %d", ip_address.c_str(), port);
    }
    this->_server_count = this->_listening_sockets.size() + 3;
}

void Server::acceptConnections() {
    std::map<int, SocketWrapper>::iterator server = this->_listening_sockets.begin();

    for (int i = 0; server != this->_listening_sockets.end(); server++) {
        if (this->_multiplexer->canRead(server->first)) {
            int new_client = server->second.acceptSocket();
            if (new_client == -1 || new_client == SERVICE_UNAVAILABLE_STATUS) {
                continue;
            }
            Logger::get().log(DEBUG, "New client connection [%d] on server on port %d",
                              (new_client - this->_server_count), server->second.getPort());
            this->_multiplexer->addFd(new_client, POLLIN);
            this->_clients.push_back(Client(new_client, this->_config.servers[i]));
        }
        i++;
    }
}

void Server::handleRequests(void) {
    std::vector<Client>::iterator client = this->_clients.begin();
    while (client != this->_clients.end()) {
		std::system("sleep 0.025");
        if (this->_multiplexer->canRead(client->getSocketFd()) && client->getIsDoneReading() == false) {
            try {
                client->read_socket();
                this->_multiplexer->addFd(client->getSocketFd(), POLLOUT);
            } catch (const std::exception &e) {
                throw std::runtime_error("Client disconnected");
            }
        }
        if (this->_multiplexer->canWrite(client->getSocketFd()) && client->getIsDoneReading() == true) {
            try {
                if (client->write_socket()) {
                    this->_multiplexer->removeFd(client->getSocketFd());
                    client->disconnect();
                    Logger::get().log(DEBUG, "Disconnecting client [%d]", client->getSocketFd() + 2);
                    client = this->_clients.erase(client);
                    continue;
                }
            } catch (const std::exception &ex) {
                this->_multiplexer->removeFd(client->getSocketFd());
                client->disconnect();
                client = this->_clients.erase(client);
                continue;
            }
        }
        client++;
    }
}
