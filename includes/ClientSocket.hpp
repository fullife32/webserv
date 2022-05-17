#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>

/*
	This class contains all informations needed to create and work with a
	client socket, to send and recv packages. It basically contains its fd, the
	client datas received with accept() and the events waited.
*/
class ClientSocket {
private:
	int					_fd;
	sockaddr_storage 	_cli;
	epoll_event			_event;

public:
	ClientSocket(int fd, sockaddr_storage cli) : _fd(fd), _cli(cli) {
		_event.events = EPOLLIN;
		_event.data.fd = _fd;
	}
	~ClientSocket() {}
	// ClientSocket( ClientSocket const &other );
	// ClientSocket &operator=( ClientSocket const &other );

	/* Close socket */
	void	close() {
		::close(_fd);
	}

	/* Getter functions */
	int	getSockfd() const {
		return _fd;
	}
	epoll_event	*getEvent() {
		return &_event;
	}
	uint32_t	getEventValue() const {
		return _event.events;
	}
};

#endif
