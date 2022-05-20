#ifndef MULTIPLEX_HPP
# define MULTIPLEX_HPP

#include "Server.hpp"
#include <iostream>
#include <sys/epoll.h>
#include <stdlib.h>
#include <map>
#include "Client.hpp"

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
	void	addServersToPoll( std::map<int, Server> &servers ) const;
	void	addClientToPoll( Client &client ) const;
	int		waitPlex();
	void	handleEvents( std::map<int, Server> &servers, std::map<int, Client> &clients );
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
	// void freeEvents() {
	// 	if (m_events)
	// 		::free(m_events);
	// }
};

#endif
