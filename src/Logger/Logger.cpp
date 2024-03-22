/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:50:52 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 16:27:00 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

Logger::Logger(std::string log_file, e_logState state ,e_logType type) : _log_file(log_file), _log_state(state), _log_type(type){
	return;
}

Logger::~Logger(void) {
	return;
}

void log(std::string message, e_logLevel level){
	(void)message;
	(void)level;
}

std::string get_curr_time(void){
	tzset();
	char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    tm.tm_hour = tm.tm_hour + GST;
    strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]\t", &tm);
    return (std::string(date));
}


