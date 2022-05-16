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
#include "parseConfig.hpp"
#include <sys/epoll.h>

class serverSocket : public serverBlock {
private:
	int				_sockfd;
	int				_yes;
	epoll_event		_ev;
	serverBlock		_block;

public:
	serverSocket(serverBlock block) : _block(block) {
		_yes = 1;
		_ev.events = EPOLLIN;
		// _ev.data.fd = fill here corresponding sockfd for a client event
		_createSocket();
		showInfos();
		_setOpts();
		_bindSocket();
		_listenSocket();
	}
	~serverSocket() {}

	void	showInfos() {
		std::cout << std::endl << "---------- Infos Socket ----------" << std::endl;
		std::cout << "sockfd: " << _sockfd << std::endl;
		std::cout << "yes: " << _yes << std::endl;
		std::cout << "block.ip_addr: " << _block.getIp() << std::endl;
		std::cout << "block.listen: " << _block.getPort() << std::endl;
		std::cout << "----------------------------------" << std::endl << std::endl;
	}

	int	getSockfd() const {
		return _sockfd;
	}
	epoll_event	*getEv() {
		return &_ev;
	}
	uint32_t	getEvent() {
		return _ev.events;
	}
	void		close() {
		if (_sockfd != -1)
			::close(_sockfd);
	}

private:
	void	_createSocket() {
		this->_sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if (this->_sockfd == -1) {
			std::cout << "Fail creating socket" << std::endl;
			exit(1);
		}
		std::cout << "Socket successfully created with fd: " << this->_sockfd << std::endl;
	}
	void	_setOpts() {
		int	ret;

		if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &this->_yes, sizeof(int)) == -1) {
			std::cout << "Fail setting options for socket: " << _sockfd << std::endl; // throw exceptions instead
			exit(1);
		}
		std::cout << "Options successfully set for socket: " << this->_sockfd << std::endl;
	}
	void	_bindSocket() {
		struct sockaddr_in sin;

		sin.sin_family = AF_INET;
		sin.sin_port = htons(atoi(_block.getPort().c_str()));
		sin.sin_addr.s_addr = inet_addr(_block.getIp().c_str());
		if (bind(this->_sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
			std::cout << "Fail binding with the socket: " << _sockfd << std::endl;
			exit(1);
		}
		std::cout << "Binding successfully made for socket: " << this->_sockfd << std::endl;
	}
	void	_listenSocket() {
		if (listen(_sockfd, 5) == -1) {
			std::cout << "Fail listening socket: " << _sockfd << std::endl;
			exit(1);
		}
		std::cout << "Socket now listening: " << this->_sockfd << std::endl;
	}
};

#endif