/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:50:52 by maroy             #+#    #+#             */
/*   Updated: 2024/05/21 15:00:21 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

Logger::Logger(std::string log_file, e_logState state, e_logType type, e_logLevel level)
    : _log_filename(log_file), _log_state(state), _log_level(level), _log_type(type) {
    _log_file.open(_log_filename, std::ios::app);
}

Logger::~Logger(void) {
    if (this->_log_file.is_open())
        this->_log_file.close();
    return;
}

Logger& Logger::get(void)
{
	static Logger logger("webserv.log");
	return (logger);
}

void Logger::destroy(void) {
    Logger& logger = get();
    if (logger._log_file.is_open()) {
        logger._log_file.close();
    }
}

void Logger::log(e_logLevel level, const char *format, ...) {
	if (level < this->_log_level)
		return;

    std::string log_level;
    std::string log_color;

    switch (level) {
    case DEBUG:
        log_level = "DEBUG";
        log_color = MAGENTA;
        break;
    case INFO:
        log_level = "INFO";
        log_color = CYAN;
        break;
    case WARN:
        log_level = "WARN";
        log_color = YELLOW;
        break;
    case ERROR:
        log_level = "ERROR";
        log_color = RED;
        break;
    }

    va_list args;
    va_start(args, format);
    int size_needed = vsnprintf(NULL, 0, format, args) + 1;
    va_end(args);

    char *message = new char[size_needed];

    va_start(args, format);
    vsnprintf(message, size_needed, format, args);
    va_end(args);


    std::string log_message = this->_get_curr_time() + "[" + log_level + "]\t" + message;
    std::string log_tty_message = this->_get_curr_time() + log_color + "[" + log_level + "]\t" + RESET_COLOR + message;

    if (this->_log_state == ENABLED) {
        if (this->_log_type == CONSOLE || this->_log_type == BOTH)
            std::cout << log_tty_message << std::endl;
        if (this->_log_type == OUT_FILE || this->_log_type == BOTH)
            this->_log_file << log_message << std::endl;
    }
    delete[] message;
}

std::string Logger::_get_curr_time(void) {
    tzset();
    char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    tm.tm_hour = (tm.tm_hour + 24 + GST) % 24;
    strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]\t", &tm);
    return (std::string(date));
}
