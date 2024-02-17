/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:50:28 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 03:57:50 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#pragma once

/********************/
/* parsing_conf.cpp */
/********************/
int parse_conf(const char *file);

/*********************/
/* parsing_utils.cpp */
/*********************/
bool brackets_balance(const std::string &str);
std::string trim(const std::string &str);

#endif // PARSING_H