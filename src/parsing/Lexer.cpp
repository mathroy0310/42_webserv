/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:39:42 by maroy             #+#    #+#             */
/*   Updated: 2024/02/15 16:42:25 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Lexer::Lexer()
{
}

Lexer::Lexer( const Lexer & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Lexer::~Lexer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Lexer &				Lexer::operator=( Lexer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Lexer const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
