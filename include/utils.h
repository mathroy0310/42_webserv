/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 02:29:38 by maroy             #+#    #+#             */
/*   Updated: 2024/05/03 00:19:22 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include "defines.h"

/************/
/* trim.cpp */
/************/

std::string trim(const std::string &str);

/******************/
/* split_line.cpp */
/******************/

std::vector<std::string> split_line(std::string line, std::string delim);

/*************************/
/* directory_listing.cpp */
/*************************/

std::string directory_listing(DIR *dir, std::string root);

/**************/
/* itohex.cpp */
/**************/

std::string itohex(int n);

#endif  // UTILS_H