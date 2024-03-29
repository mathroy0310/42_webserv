/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:04:22 by maroy             #+#    #+#             */
/*   Updated: 2024/03/26 16:31:12 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

Logger g_logger("webserv.log");

int main(int argc, char **argv) {
    if (argc == 1)
        argv[1] = (char *)"./conf/def.conf";
    if (argc > 2) {
        std::cerr << ERR_MSG_USAGE(argv[0]) << FILE_LINE;
        return EXIT_FAILURE;
    }

    g_logger.log(INFO, "Logger is Enabled");
    t_config config = parse_conf(argv[1]);
    g_logger.log(INFO, "Config file parsed successfully");
    Server server(config);
    server.run();
    
    return EXIT_SUCCESS;
}
