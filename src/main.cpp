/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:04:22 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 03:45:45 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "parsing.h"

int main(int argc, char **argv)
{
	if (argc == 1)
		argv[1] = (char *)"./conf/def.conf";
	if (argc > 2)
	{
		std::cerr << ERR_MSG_USAGE(argv[0]) << FILE_LINE;
		return EXIT_FAILURE;
	}

	return (parse_conf(argv[1]));
}
