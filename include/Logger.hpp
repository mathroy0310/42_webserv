/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:50:55 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 18:08:41 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include "defines.h"
# include "parsing.hpp"

# include <iostream>
# include <fstream>

#define GST -4

enum e_logLevel
{
	DEBUG,
	INFO,
	WARNING,
	ERROR
};

enum e_logType
{
	BOTH,
	CONSOLE,
	OUT_FILE
};

enum e_logState
{
	ENABLED,
	DISABLED
};

class Logger
{
	public:
		Logger(std::string log_file, e_logState state = ENABLED ,e_logType type = BOTH);
		~Logger(void);
		void log(std::string message, std::string arg = "", e_logLevel level = INFO);
 
	private:
		
		const std::string 	_log_filename;
		const e_logState	_log_state;
		const e_logType		_log_type;
		std::ofstream _log_file;
		std::string _get_curr_time(void);

};

#endif // LOGGER_HPP
