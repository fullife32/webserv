#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "Socket.hpp"

/*
	This class contains all informations needed to create and work with a
	client socket, to send and recv packages. It basically contains its fd, the
	client datas received with accept() and the events waited.
*/
class Client : public Socket {
private:
	sockaddr_storage 	m_cli;
	socklen_t			m_size;

public:
	Client( int fd, sockaddr_storage cli, socklen_t size );
	Client( Client const &other );
	~Client();

private:
	Client();
	Client &operator=( Client const &other );

public:

	static Client	acceptClient( int fdServer );

	class ClientFail : public std::exception {
	public:
		ClientFail() { }
		virtual const char	*what() const throw() {
			return "Client create failed";
		}
	};
};

#endif
