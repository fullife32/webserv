#ifndef MULTIPLEX_HPP
# define MULTIPLEX_HPP

#include "ServerSocket.hpp"
#include <iostream>
#include <sys/epoll.h>
#include <stdlib.h>

#define MAXEVENTS 64

/*
	This class create an epoll list and have functions to add a new socket to
	it, modify events associated with socket and waiting all the events in the
	list to happen.
*/
class Multiplex {
private:
	int			m_epfd;
	int			m_nbReady;
	epoll_event *m_events;

public:
	Multiplex() {
		if (m_createPlex() == -1)
			exit(1); // don't exit
	}
	~Multiplex() {}
	// Multiplex( Multiplex const &other );
	// Multiplex &operator=( Multiplex const &other );

	/*
		Adding a socket to the epoll list, might be a server socket or a client
		socket. It need the file descriptor of the epoll list, the event ADD,
		the fd of the socket to watch and an event struct containing the events
		wanted and the socket own fd.
	*/
	int	addToPoll(int fd) {
		epoll_event	event;

		if (fd == -1)
			std::cout << "Set invalid fd to addToPoll" << std::endl;
		event.events = EPOLLIN;
		event.data.fd = fd;
		if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &event) == -1) {
			std::cout << "epoll_ctl failed" << std::endl;
			return -1;
		}
		std::cout << "epoll_ctl successfully applied for socket: " << fd << std::endl;
		return 0; //secure main
	}
	/*
		This wait function wait until an event contained in the list happen for
		a socket. It need the epoll fd, an allocated list the size of MAXEVENTS
		that the function will fill with the events that occured and their fd,
		the size of the allocated list and the timeout, in this case -1 to
		wait infinitely.
		If there are more that events than MAXEVENTS, the events will wait the
		next call of the function to be added in the allocated list.
	*/
	int waitPlex() {
		m_events = (epoll_event *)calloc(MAXEVENTS, sizeof(m_events));
		if (!m_events) {
			std::cout << "calloc failed in waitPlex" << std::endl;
			exit(1); // don't exit
		}
		std::cout << "Start waiting" << std::endl;
		m_nbReady = epoll_wait(m_epfd, m_events, MAXEVENTS, EPOLL_TIMEOUT); //replace 10 by total nb of events
		if (m_nbReady == -1) {
			std::cout << "epoll_wait failed" << std::endl;
			return -1;
		}
		std::cout << "Finished waiting" << std::endl;
		return 0;
	}
	/*
		This function will check every elements in an event list and redirect
		to function according to the type of the event and to the type of the
		socket.
		- If it's a server socket, the event always being EPOLLIN, it will
		redirect to a function that will accept a new client connection.
		- If it's a client socket, it will check if it's an event IN or OUT and
		will redirect, to a function that handles http request for the first
		case, or to a function that handles https response for the second case.
	*/
	int	watchEvents(ServerSocket &sockets) {
		for (int i = 0; i < m_nbReady; i++) {
			if (m_events[i].data.fd == sockets.getFd()) {
				if (!(m_events[i].events & EPOLLIN))
					std::cout << "Fail occured in watchEvents" << std::endl;
				else if (m_events[i].data.fd == sockets.getFd())
					addToPoll(sockets.addClient());
				if (sockets.getClientFd(m_events[i].data.fd) == -1)
					std::cout << "Fail finding client after epoll wait" << std::endl;
				else
					std::cout << "Coucou !" << std::endl;
			}
		}
		this->freeEvents();
		return 0;
	}
	void freeEvents() {
		if (m_events)
			::free(m_events);
	}

private:
	/*
		Epoll_create1 create the epoll list where we can add, modify or delete
		socket and events associated to it. It take only one argument with is a
		flag that automatically close the socket fd if an exec function occured
		on it (Not neccessary ?).
	*/
	int	m_createPlex() {
		m_epfd = epoll_create1(0); //FD_CLOEXEC if need to close after exec
		if (m_epfd == -1) {
			std::cout << "epoll_create1 failed" << std::endl;
			return -1;
		}
		std::cout << "epoll_create1 successfully created with fd: " << m_epfd << std::endl;
		return 0;
	}
};

#endif
