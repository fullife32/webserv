#ifndef MULTIPLEX_HPP
# define MULTIPLEX_HPP

#include "Server.hpp"
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
	int			m_fd;
	int			m_nbReady;
	epoll_event	*m_events;

public:
	Multiplex();
	~Multiplex();

private:
	Multiplex( Multiplex const &other );
	Multiplex &operator=( Multiplex const &other );
	
public:
	void	createPlex();
	void	addServersToPoll( std::vector<Server> &servers ) const;
	void	closePlex();

	class PlexFail : public std::exception {
	public:
		PlexFail() { }
		virtual const char	*what() const throw() {
			return "Plex create failed";
		}
	};

	/*
		Adding a socket to the epoll list, might be a server socket or a client
		socket. It need the file descriptor of the epoll list, the event ADD,
		the fd of the socket to watch and an event struct containing the events
		wanted and the socket own fd.
	*/
	// int	addToPoll(int fd) {
	// 	epoll_event	event;

	// 	if (fd == -1)
	// 		std::cout << "Set invalid fd to addToPoll" << std::endl;
	// 	event.events = EPOLLIN;
	// 	event.data.fd = fd;
	// 	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &event) == -1) {
	// 		std::cout << "epoll_ctl failed" << std::endl;
	// 		return -1;
	// 	}
	// 	std::cout << "epoll_ctl successfully applied for socket: " << fd << std::endl;
	// 	return 0; //secure main
	// }
	/*
		This wait function wait until an event contained in the list happen for
		a socket. It need the epoll fd, an allocated list the size of MAXEVENTS
		that the function will fill with the events that occured and their fd,
		the size of the allocated list and the timeout, in this case -1 to
		wait infinitely.
		If there are more that events than MAXEVENTS, the events will wait the
		next call of the function to be added in the allocated list.
	*/
	// int waitPlex() {
	// 	m_events = (epoll_event *)calloc(MAXEVENTS, sizeof(m_events));
	// 	if (!m_events) {
	// 		std::cout << "calloc failed in waitPlex" << std::endl;
	// 		exit(1); // don't exit
	// 	}
	// 	std::cout << "Start waiting" << std::endl;
	// 	m_nbReady = epoll_wait(m_epfd, m_events, MAXEVENTS, EPOLL_TIMEOUT); //replace 10 by total nb of events
	// 	if (m_nbReady == -1) {
	// 		std::cout << "epoll_wait failed" << std::endl;
	// 		return -1;
	// 	}
	// 	std::cout << "Finished waiting" << std::endl;
	// 	return 0;
	// }
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
	// int	watchEvents(Server &sockets) {
	// 	for (int i = 0; i < m_nbReady; i++) {
	// 		if (m_events[i].data.fd == sockets.getFd()) {
	// 			if (!(m_events[i].events & EPOLLIN))
	// 				std::cout << "Fail occured in watchEvents" << std::endl;
	// 			else if (m_events[i].data.fd == sockets.getFd())
	// 				addToPoll(sockets.addClient());
	// 			if (sockets.getClientFd(m_events[i].data.fd) == -1)
	// 				std::cout << "Fail finding client after epoll wait" << std::endl;
	// 			else
	// 				std::cout << "Coucou !" << std::endl;
	// 		}
	// 	}
	// 	this->freeEvents();
	// 	return 0;
	// }
	// void freeEvents() {
	// 	if (m_events)
	// 		::free(m_events);
	// }
};

#endif
