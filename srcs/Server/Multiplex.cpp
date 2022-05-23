/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 15:36:30 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/23 15:27:57 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplex.hpp"

Multiplex::Multiplex() : m_fd(-1), m_nbReady(0), m_events(NULL) { }

Multiplex::~Multiplex() {
	freeEvents();
	closePlex();
}

void	Multiplex::createPlex() {
	m_fd = epoll_create1(0); //FD_CLOEXEC if need to close after exec
	if (m_fd == -1)
		throw Multiplex::PlexFail();
}

void	Multiplex::addServersToPoll( std::map<int, Server> &servers ) const {
	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it) {
		epoll_event	event;

		if (it->second.getFd() == -1)
			throw Multiplex::PlexFail(); // change message
		memset(&event, 0, sizeof(event));
		event.events = EPOLLIN;
		event.data.fd = it->second.getFd();
		if (epoll_ctl(m_fd, EPOLL_CTL_ADD, it->second.getFd(), &event) == -1) // check error uninitialised byte(s)
			throw Multiplex::PlexFail(); // change message
	}
}

void	Multiplex::addClientToPoll( Client &client ) const {
	epoll_event	event;

	if (client.getFd() == -1)
		throw Multiplex::PlexFail(); // change message
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = client.getFd();
	if (epoll_ctl(m_fd, EPOLL_CTL_ADD, client.getFd(), &event) == -1) // check error uninitialised byte(s)
		throw Multiplex::PlexFail(); // change message
}

int	Multiplex::waitPlex() {
	m_events = new epoll_event[MAXEVENTS];
	if (m_events == NULL) {
		std::cerr << "new failed in waitPlex" << std::endl;
		return -1;
	}
	m_nbReady = epoll_wait(m_fd, m_events, MAXEVENTS, EPOLL_TIMEOUT);
	if (m_nbReady == -1) {
		std::cerr << "epoll_wait failed" << std::endl;
		return -1;
	}
	return m_nbReady;
}

void	Multiplex::handleEvents( std::map<int, Server> &servers, std::map<int, Client> &clients ) {
	for (int i = 0; i < m_nbReady; i++) {
		handleServer(i, servers, clients);
		handleClients(i, clients);
	}
	freeEvents();
}

void	Multiplex::handleServer( int i, std::map<int, Server> &servers, std::map<int, Client> &clients ) {
	std::map<int, Server>::iterator it = servers.find(m_events[i].data.fd);
	if (it != servers.end()) {
		if (!(m_events[i].events & EPOLLIN)) // secure client too
			std::cerr << "Ghost event in server: " << it->second.getFd() << std::endl; // return error ?
		else {
			try {
				Client	tmpClient(Client::acceptClient(it->second.getFd(), it->second));
				clients.insert(std::make_pair(tmpClient.getFd(), tmpClient));
				addClientToPoll(clients.rbegin()->second);
				// Debug
				std::cout << clients.rbegin()->second.getFd() << ": coucou je suis nouveau ici !" << std::endl;
				//
			} catch(std::exception &except) {
				std::cerr << except.what() << std::endl; // check to change to cerr not cout
			}
		}
	}
}

void	Multiplex::handleClients( int i, std::map<int, Client> &clients ) {
	std::map<int, Client>::iterator it = clients.find(m_events[i].data.fd);
	if (it != clients.end()) {
		if (m_events[i].events & EPOLLIN) {
			epoll_event	event;

			// Debug
			std::cout << it->second.getFd() << ": ciao, je me casse !" << std::endl;
			//
			memset(&event, 0, sizeof(event));
			epoll_ctl(m_fd, it->second.getFd(), EPOLL_CTL_DEL, &event); // put in function kick
			it->second.closeSocket();
			clients.erase(it);
		}
		else if (m_events[i].events & EPOLLOUT) {

		}
	}
}

void	Multiplex::freeEvents() {
	if (m_events)
		delete [] m_events;
}

void	Multiplex::closePlex() { // delete everything before closing
	if (m_fd != -1)
		close(m_fd);
	freeEvents();
}
