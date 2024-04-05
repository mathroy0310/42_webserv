/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 02:29:38 by maroy             #+#    #+#             */
/*   Updated: 2024/04/05 03:10:54 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include "defines.h"

/*******************/
/* trim.cpp */
/*******************/

std::string trim(const std::string &str);

/*******************/
/* split_line.cpp */
/*******************/

std::vector<std::string> split_line(std::string line, std::string delim);

#endif  // UTILS_H