#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "ParseConfig.hpp"
#include "ClientSocket.hpp"
#include <sys/epoll.h>
#include <iterator>

#define LISTEN_LEN 1024
#define EPOLL_TIMEOUT -1

class Multiplex;

/*
	This class contains all informations needed to create and work with a
	server socket. Its file descriptor, the event associated will always be
	EPOLLIN to listen for new clients connections, its server block infos and a
	vector of all its clients.
*/
class ServerSocket {
private:
	int							_fd;
	int							_yes;
	epoll_event					_event; // remove this and apply in when added in epoll only
	ServerBlock					&_block;
	std::vector<ClientSocket>	_clients;

public:
	/*
		The constructor create the socket and make it listen.
	*/
	ServerSocket(ServerBlock &block) : _block(block) {
		_yes = 1;
		_event.events = EPOLLIN;
		_createSocket();
		showInfos();
		_setOpts();
		_bindSocket();
		_listenSocket();
		_event.data.fd = _fd;
	}
	~ServerSocket() {}
	// ServerSocket( ServerSocket const &other );
	// ServerSocket &operator=( ServerSocket const &other );

	void	showInfos() {
		std::cout << std::endl << "---------- Infos Socket ----------" << std::endl;
		std::cout << "sockfd: " << _fd << std::endl;
		std::cout << "yes: " << _yes << std::endl;
		std::cout << "block.ip_addr: " << _block.getIp() << std::endl;
		std::cout << "block.listen: " << _block.getPort() << std::endl;
		std::cout << "clients:" << std::endl;
		for (std::vector<ClientSocket>::iterator it = _clients.begin() ; it != _clients.end(); ++it)
			std::cout << "fd: " << it->getFd() << std::endl;
		std::cout << "----------------------------------" << std::endl << std::endl;
	}
	/*
		Not finished.
	*/
	void	addClient(Multiplex *plex) {
		sockaddr_storage	cli;
		socklen_t			size;
		int					newFd;

		size = sizeof(cli);
		newFd = accept(_fd, (struct sockaddr *)&cli, &size);
		if (newFd == -1)
			std::cout << "Fail adding new client" << std::endl;
		else {
			ClientSocket newClient(newFd, cli);
			_clients.push_back(newClient);
			std::cout << "Successfully added client: " << newClient.getFd() << std::endl;
			// plex->addToPoll(newFd, newClient.getEvent());
		}
	}
	/* Close socket */
	void	close() {
		if (_fd != -1)
			::close(_fd);
	}

	/* Getter functions */
	int	getFd() const {
		return _fd;
	}
	epoll_event	*getEvent() {
		return &_event;
	}
	uint32_t	getEventValue() const {
		return _event.events;
	}

private:
	/*
		Function socket needs the ip protocol, in this case IPv4 so PF_INET,
		the transport protocol, TCP so SOCK_STREAM with no flag and return a
		socket file descriptor.
	*/
	void	_createSocket() {
		this->_fd = socket(PF_INET, SOCK_STREAM, 0);
		if (this->_fd == -1) {
			std::cout << "Fail creating socket" << std::endl;
			exit(1);
		}
		std::cout << "Socket successfully created with fd: " << this->_fd << std::endl;
	}
	/*
		This function set the socket ip/port to be reusable after finishing using
		it. It takes the socket file descriptor, at option level socket so
		SOL_SOCKET and make ip/port reusable with SO_REUSEADDR with an
		int yes = 1 to accept it or not.
	*/
	void	_setOpts() {
		int	ret;

		if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &this->_yes, sizeof(int)) == -1) {
			std::cout << "Fail setting options for socket: " << _fd << std::endl; // throw exceptions instead
			exit(1);
		}
		std::cout << "Options successfully set for socket: " << this->_fd << std::endl;
	}
	/*
		This function bind the socket to an ip and a port if possible. It takes
		a structure with 3 datas, the IP family, AF_INET for IPv4, the port
		transformed to a network form with htons() and an ip transform to a
		network form too with inet_addr().
	*/
	void	_bindSocket() {
		struct sockaddr_in sin;

		sin.sin_family = AF_INET;
		sin.sin_port = htons(_block.getPort());
		sin.sin_addr.s_addr = inet_addr(_block.getIp());
		if (bind(this->_fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
			std::cout << "Fail binding with the socket: " << _fd << std::endl;
			exit(1);
		}
		std::cout << "Binding successfully made for socket: " << this->_fd << std::endl;
	}
	/*
		This function make the socket port open for listening to accept new
		clients, LISTEN_LEN corresponding to the waiting queue max size, in our
		case, it's 1024.
	*/
	void	_listenSocket() {
		if (listen(_fd, LISTEN_LEN) == -1) {
			std::cout << "Fail listening socket: " << _fd << std::endl;
			exit(1);
		}
		std::cout << "Socket now listening: " << this->_fd << std::endl;
	}
};

#endif