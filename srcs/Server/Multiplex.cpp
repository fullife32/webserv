/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 15:36:30 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/08 14:21:35 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplex.hpp"

Multiplex::Multiplex()
: m_signal_fd(-1), m_fd(-1), m_nbReady(0), m_events(NULL) { }

Multiplex::~Multiplex() {
	closePlex();
}

void	Multiplex::createPlex() {
	m_fd = epoll_create1(0); //FD_CLOEXEC if need to close after exec
	if (m_fd == -1)
		throw Multiplex::PlexFail();
	handleSignal();
	std::cout << m_fd << ": epoll list created" << std::endl; // TODO DEBUG
}

int	Multiplex::waitPlex() {
	m_events = new epoll_event[MAXEVENTS];
	if (m_events == NULL) {
		std::cerr << "new failed in waitPlex" << std::endl;
		return -1;
	}
	m_nbReady = epoll_wait(m_fd, m_events, MAXEVENTS, EPOLL_TIMEOUT);
	if (m_nbReady == -1) {
		std::cout << "Something went wrong on the server selector" << std::endl;
		return -1;
	}
	return m_nbReady;
}

int	Multiplex::handleEvents( std::map<int, Server> &servers, std::map<int, Client> &clients ) {
	for (int i = 0; i < m_nbReady; i++) {
		if (m_events[i].data.fd == m_signal_fd) {
			std::cout << "Stopping server peacefully..." << std::endl;
			return -1;
		}
		handleServer(i, servers, clients); // TODO cut search in two with epoll fd, < servers, > clients ?
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
		throw Multiplex::PlexFail();
}

void	Multiplex::handleSignal() {
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGTERM);
	sigaddset(&mask, SIGINT);
	int	ret = sigprocmask(SIG_BLOCK, &mask, 0);
	if (ret == -1)
		throw Multiplex::PlexFail();
	m_signal_fd = signalfd(-1, &mask, 0);
	if (m_signal_fd == -1)
		throw Multiplex::PlexFail();
	addSignalToPoll();
}



void	Multiplex::addServersToPoll( std::map<int, Server> &servers ) const {
	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it) {
		epoll_event	event;

		if (it->second.getFd() == -1)
			throw Multiplex::PlexFail(); // change message
		memset(&event, 0, sizeof(event));
		event.events = EPOLLIN;
		event.data.fd = it->second.getFd();
		if (epoll_ctl(m_fd, EPOLL_CTL_ADD, it->second.getFd(), &event) == -1)
			throw Multiplex::PlexFail(); // change message
	}
}

void	Multiplex::handleServer( int i, std::map<int, Server> &servers, std::map<int, Client> &clients ) {
	std::map<int, Server>::iterator it = servers.find(m_events[i].data.fd);
	if (it != servers.end()) {
		if (!(m_events[i].events & EPOLLIN)) // secure client too
			std::cerr << "Ghost event in server: " << it->second.getFd() << std::endl; // return error ?
		else {
			try {
				Client	tmpClient(Client::acceptClient(it->second.getFd(), it->second));
			std::cout << tmpClient.getFd() << std::endl;
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



void	Multiplex::addClientToPoll( Client &client ) const {
	epoll_event	event;

	if (client.getFd() == -1)
		throw Multiplex::ClientFail(); // change message
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = client.getFd();
	if (epoll_ctl(m_fd, EPOLL_CTL_ADD, client.getFd(), &event) == -1)
		throw Multiplex::ClientFail(); // change message
}

/*
	iterates on the client events:
		EPOLLIN : the client sends a request
		EPOLLOUT : the client waits for a response
*/
void	Multiplex::handleClients( int i, std::map<int, Client> &clients ) {
	std::map<int, Client>::iterator currentClient = clients.find(m_events[i].data.fd);
	if (currentClient != clients.end()) {
		if (m_events[i].events & EPOLLIN) {
			currentClient->second.receive_data();
			m_checkClientChangeEvent(currentClient, clients);
		}
		else if (m_events[i].events & EPOLLOUT) {
			currentClient->second.send_data();
			m_checkClientChangeEvent(currentClient, clients);
		}
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
		} catch (Multiplex::ClientFail const &except) {
			std::cerr << except.what() << std::endl;
		}
	}
	else if (currentClient->second.getToRemove()) {
		try {
			deleteClient(clients, currentClient);
		} catch (Multiplex::ClientFail const &except) {
			std::cerr << except.what() << std::endl;
		}
	}
}

void	Multiplex::changeClientEvent( Client &client, int newEvent ) const { // where to call this function ?
	epoll_event	event;

	if (client.getFd() == -1)
		throw Multiplex::ClientFail(); // change message
	memset(&event, 0, sizeof(event));
	event.events = newEvent;
	event.data.fd = client.getFd();
	if (epoll_ctl(m_fd, EPOLL_CTL_MOD, client.getFd(), &event) == -1)
		throw Multiplex::ClientFail(); // change message
	client.setToChangeEvent();
}

void	Multiplex::deleteClient( std::map<int, Client> &clients, std::map<int, Client>::iterator it ) {
	// Debug
	std::cout << it->second.getFd() << ": ciao, je me casse !" << std::endl;
	//
	if (epoll_ctl(m_fd, EPOLL_CTL_DEL, it->first, NULL) == -1) {
		throw Multiplex::ClientFail(); // change message
	}
	it->second.closeSocket();
	clients.erase(it);
}



void	Multiplex::freeEvents() {
	if (m_events)
		delete [] m_events;
}

void	Multiplex::closePlex() { // delete everything before closing
	freeEvents();
	if (m_fd != -1)
		close(m_fd);
	if (m_signal_fd != -1)
		close(m_signal_fd);
}
