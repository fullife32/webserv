
#include <iostream>
#include <sys/epoll.h>
#include "Webserv.hpp"

int	plexSocket(ServerSocket socket) {
	int	epfd;
	epfd = epoll_create1(0); //FD_CLOEXEC if need to close after exec
	if (epfd == -1) {
		std::cout << "epoll_create1 failed" << std::endl;
		return -1;
	}
	std::cout << "epoll_create1 successfully created with fd: " << epfd << std::endl;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket.getSockfd(), socket.getEvent()) == -1) {
		std::cout << "epoll_create1 failed" << std::endl;
		return -1;
	}
	std::cout << "epoll_ctl successfully applied for socket: " << socket.getSockfd() << std::endl;
	return epfd;
}
