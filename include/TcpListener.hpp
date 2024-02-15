#ifndef TCPLISTENER_HPP
# define TCPLISTENER_HPP

# include <iostream>
# include <string>

class TcpListener
{

	public:

		TcpListener();
		TcpListener( TcpListener const & src );
		~TcpListener();

		TcpListener &		operator=( TcpListener const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, TcpListener const & i );

#endif /* ***************************************************** TCPLISTENER_H */