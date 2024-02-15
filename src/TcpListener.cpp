#include "TcpListener.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

TcpListener::TcpListener()
{
}

TcpListener::TcpListener( const TcpListener & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

TcpListener::~TcpListener()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

TcpListener &				TcpListener::operator=( TcpListener const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, TcpListener const & i )
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