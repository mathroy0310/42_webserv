/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:04:22 by maroy             #+#    #+#             */
/*   Updated: 2024/05/17 14:13:31 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "Server/Server.hpp"

int main(int argc, char **argv) {
    if (argc == 1)
        argv[1] = (char *)"./conf/def.conf";
    if (argc > 2) {
        std::cerr << ERR_MSG_USAGE(argv[0]) << FILE_LINE;
        return EXIT_FAILURE;
    }
    t_config config = parse_conf(argv[1]);
    Logger::get().log(INFO, "Logger is Enabled");
    Logger::get().log(DEBUG, "Config file parsed successfully");
    Server server(config);
    server.run();

    return EXIT_SUCCESS;
}
