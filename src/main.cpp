/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:04:22 by maroy             #+#    #+#             */
/*   Updated: 2024/05/12 00:38:59 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "Server/Server.hpp"

Server* globalServer = nullptr;

// Signal handler function
void handleSignal(int signal) {
    if (signal == SIGINT) {
        Logger::destroy(); 
        if (globalServer) {
            globalServer->stop();
        }
        std::exit(EXIT_SUCCESS);
    }
}

int main(int argc, char **argv) {
    std::signal(SIGINT, handleSignal);
    if (argc == 1)
        argv[1] = (char *)"./conf/def.conf";
    if (argc > 2) {
        std::cerr << ERR_MSG_USAGE(argv[0]) << FILE_LINE;
        return EXIT_FAILURE;
    }
    Logger::get().log(INFO, "Logger is Enabled");
    t_config config = parse_conf(argv[1]);
    Logger::get().log(INFO, "Config file parsed successfully");
    Server server(config);
    globalServer = &server; 
    server.run();

    Logger::destroy();
    return EXIT_SUCCESS;
}
