#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/epoll.h>

/*
	This class serve as an assembly of all the basic infos of a socket,
	compatible with a server and a client.
*/
class Socket {
private:
	int	_socketFd;
public:
	Socket();
	~Socket();
	// Socket( Socket const &other );
	// Socket &operator=( Socket const &other );
	int	getFd() const {
		return _socketFd;
	}
};

#endif