/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:50:52 by maroy             #+#    #+#             */
/*   Updated: 2024/03/23 01:22:43 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

Logger::Logger(std::string log_file, e_logState state ,e_logType type) : _log_filename(log_file), _log_state(state), _log_type(type){
	_log_file.open(_log_filename, std::ios::app);
	return;
}

Logger::~Logger(void) {
	this->_log_file.close();
	return;
}

void Logger::log(std::string message, std::string arg, e_logLevel level){
	std::string log_level;
	std::string log_color;

	switch (level)
	{
	case DEBUG:
		log_level = "DEBUG";
		log_color = MAGENTA;
		break;
	case INFO:
		log_level = "INFO";
		log_color = CYAN;
		break;
	case WARNING:
		log_level = "WARNING";
		log_color = YELLOW;
		break;
	case ERROR:
		log_level = "ERROR";
		log_color = RED;
		break;
	}

	std::string log_message = this->_get_curr_time() + "[" + log_level + "]\t" + message + " " + arg;
	std::string log_tty_message = log_color + "[" + log_level + "]\t"+ RESET_COLOR + message + " " + GREY + arg;
	if (this->_log_state == ENABLED)
	{
		if (this->_log_type == CONSOLE || this->_log_type == BOTH)
			std::cout << log_tty_message << std::endl;
		if (this->_log_type == OUT_FILE || this->_log_type == BOTH)
			this->_log_file << log_message << std::endl;
	}
}

std::string Logger::_get_curr_time(void){
	tzset();
	char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    tm.tm_hour = tm.tm_hour + GST;
    strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]\t", &tm);
    return (std::string(date));
}


