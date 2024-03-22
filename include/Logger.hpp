/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:50:55 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 16:27:00 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include "defines.h"
# include "parsing.hpp"

#define GST 1

enum e_logLevel
{
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
		Logger(std::string log_file, e_logState state ,e_logType type);
		~Logger(void);

	private:
		
		const std::string _log_file;
		const e_logState		_log_state;
		const e_logType		_log_type;
		
		void log(std::string message, e_logLevel level);
		std::string get_curr_time(void);

};

#endif // LOGGER_HPP
