/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:40:00 by maroy             #+#    #+#             */
/*   Updated: 2024/02/15 16:42:22 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>

class Parser
{

	public:

		Parser();
		Parser( Parser const & src );
		~Parser();

		Parser &		operator=( Parser const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Parser const & i );

#endif /* ********************************************************** PARSER_H */
