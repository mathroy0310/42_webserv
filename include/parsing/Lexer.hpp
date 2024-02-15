/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:39:54 by maroy             #+#    #+#             */
/*   Updated: 2024/02/15 16:42:23 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include <iostream>
# include <string>

class Lexer
{

	public:

		Lexer();
		Lexer( Lexer const & src );
		~Lexer();

		Lexer &		operator=( Lexer const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Lexer const & i );

#endif /* *********************************************************** LEXER_H */
