/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 15:36:30 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/15 15:26:51 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplex.hpp"

Multiplex::Multiplex()
: m_signal_fd(-1), m_fd(-1), m_nbReady(0), m_events(NULL) { }

Multiplex::~Multiplex() {
	closePlex();
}

void	Multiplex::createPlex() {
	m_fd = epoll_create1(EPOLL_CLOEXEC);
	if (m_fd == -1)
		throw Multiplex::PlexFail(PLEX_CREATE_FAIL);
	handleSignal();
}

int	Multiplex::waitPlex() {
	m_events = new epoll_event[MAXEVENTS];
	if (m_events == NULL) {
		std::cerr << "new failed in waitPlex" << std::endl;
		return -1;
	}
	m_nbReady = epoll_wait(m_fd, m_events, MAXEVENTS, EPOLL_TIMEOUT);
	if (m_nbReady == -1) {
		std::cerr << "Something went wrong on the server selector" << std::endl;
		return -1;
	}
	return m_nbReady;
}

int	Multiplex::handleEvents( std::map<int, Server> &servers, std::map<int, Client> &clients ) {
	for (int i = 0; i < m_nbReady; i++) {
		if (m_events[i].data.fd == m_signal_fd) {
			std::cerr << "Stopping server peacefully..." << std::endl;
			return -1;
		}
		handleServer(i, servers, clients);
		handleClients(i, clients);
	}
	freeEvents();
	return 0;
}



void	Multiplex::addSignalToPoll() const {
	epoll_event	event;

	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = m_signal_fd;
	if (epoll_ctl(m_fd, EPOLL_CTL_ADD, m_signal_fd, &event) == -1)
		throw Multiplex::PlexFail(PLEX_ADD_FAIL);
}

void	Multiplex::handleSignal() {
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGTERM);
	sigaddset(&mask, SIGINT);
	int	ret = sigprocmask(SIG_BLOCK, &mask, 0);
	if (ret == -1)
		throw Multiplex::PlexFail(PLEX_CREATE_FAIL);
	m_signal_fd = signalfd(-1, &mask, 0);
	if (m_signal_fd == -1)
		throw Multiplex::PlexFail(PLEX_CREATE_FAIL);
	addSignalToPoll();
}



void	Multiplex::addServersToPoll( std::map<int, Server> &servers ) const {
	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it) {
		epoll_event	event;

		if (it->second.getFd() == -1)
			throw Multiplex::PlexFail(PLEX_FD_FAIL);
		memset(&event, 0, sizeof(event));
		event.events = EPOLLIN;
		event.data.fd = it->second.getFd();
		if (epoll_ctl(m_fd, EPOLL_CTL_ADD, it->second.getFd(), &event) == -1)
			throw Multiplex::PlexFail(PLEX_ADD_FAIL);
	}
}

void	Multiplex::handleServer( int i, std::map<int, Server> &servers, std::map<int, Client> &clients ) {
	std::map<int, Server>::iterator it = servers.find(m_events[i].data.fd);
	if (it != servers.end()) {
		if (!(m_events[i].events & EPOLLIN)) // secure client too
			std::cerr << "Ghost event in server: " << it->second.getFd() << std::endl; // return error ?
		else {
			int	fd = -1;
			try {
				fd = it->second.getFd();
				Client	tmpClient(Client::acceptClient(it->second.getFd(), it->second));
				addClientToPoll(tmpClient);
				clients.insert(std::make_pair(tmpClient.getFd(), tmpClient));
				// Debug
				std::cout << clients.rbegin()->second.getFd() << ": Client connected" << std::endl;
				//
			} catch(Multiplex::PlexFail &except) {
				if (fd != -1)
					close(fd);
				std::cerr << except.what() << std::endl;
			}
		}
	}
}



void	Multiplex::addClientToPoll( Client &client ) const {
	epoll_event	event;

	if (client.getFd() == -1)
		throw Multiplex::PlexFail(PLEX_FD_FAIL);
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = client.getFd();
	if (epoll_ctl(m_fd, EPOLL_CTL_ADD, client.getFd(), &event) == -1)
		throw Multiplex::PlexFail(PLEX_ADD_FAIL);
}

/*
	iterates on the client events:
		EPOLLIN : the client sends a request
		EPOLLOUT : the client waits for a response
*/
void	Multiplex::handleClients( int i, std::map<int, Client> &clients ) {
	std::map<int, Client>::iterator currentClient = clients.find(m_events[i].data.fd);
	if (currentClient != clients.end()) {
		if (m_events[i].events & EPOLLIN)
		{
			currentClient->second.receive_data();
			
		}

		else if (m_events[i].events & EPOLLOUT)
		{
			currentClient->second.send_data();
		}
		m_checkClientChangeEvent(currentClient, clients);
	}
}

/*
	if a client has finished its request he waits for a response : the event becomes EPOLLOUT.
	if the client has received his answer : the client is deleted.
*/
void	Multiplex::m_checkClientChangeEvent( std::map<int, Client>::iterator currentClient,  std::map<int, Client> &clients) 
{
	if (currentClient->second.getToChangeEvent()) {
		try {
			changeClientEvent(currentClient->second, EPOLLOUT);
		} catch (std::exception const &except) {
			std::cerr << except.what() << std::endl;
		}
	}
	else if (currentClient->second.getToRemove()) {
		try {
			deleteClient(clients, currentClient);
		} catch (std::exception const &except) {
			std::cerr << except.what() << std::endl;
		}
	}
}

void	Multiplex::changeClientEvent( Client &client, int newEvent ) const {
	epoll_event	event;

	if (client.getFd() == -1)
		throw Multiplex::PlexFail(PLEX_FD_FAIL);
	memset(&event, 0, sizeof(event));
	event.events = newEvent;
	event.data.fd = client.getFd();
	if (epoll_ctl(m_fd, EPOLL_CTL_MOD, client.getFd(), &event) == -1)
		throw Multiplex::PlexFail(PLEX_MOD_FAIL);
	client.setToChangeEvent();
}

void	Multiplex::deleteClient( std::map<int, Client> &clients, std::map<int, Client>::iterator &it ) {
	if (epoll_ctl(m_fd, EPOLL_CTL_DEL, it->first, NULL) == -1)
		throw Multiplex::PlexFail(PLEX_DEL_FAIL);
	std::cout << it->second.getFd() << ": Client disconnected" << std::endl;
	it->second.closeSocket();
	clients.erase(it);
}



void	Multiplex::freeEvents() {
	if (m_events)
		delete [] m_events;
}

void	Multiplex::closePlex() {
	freeEvents();
	if (m_fd != -1)
		close(m_fd);
	if (m_signal_fd != -1)
		close(m_signal_fd);
}
