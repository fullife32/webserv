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
class ClientSocket : public Socket {
private:
	sockaddr_storage 	m_cli;
	socklen_t			m_size;

public:
	ClientSocket( int fd, sockaddr_storage cli, socklen_t m_size ) : Socket(fd), m_cli(cli) {}
	~ClientSocket() {}

private:
	ClientSocket( ClientSocket const &other );
	ClientSocket &operator=( ClientSocket const &other );

public:
	int	getFd() const {
		return m_fd;
	}
};

#endif
