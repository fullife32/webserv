/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 15:36:30 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/20 16:28:13 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplex.hpp"

Multiplex::Multiplex() : m_fd(-1) { }

Multiplex::~Multiplex() { }

void	Multiplex::createPlex() {
	m_fd = epoll_create1(0); //FD_CLOEXEC if need to close after exec
	if (m_fd == -1)
		throw Multiplex::PlexFail();
}

void	Multiplex::addServersToPoll( std::vector<Server> &servers ) const {
	for (std::vector<Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it) {
		epoll_event	event;

		if ((*it).getFd() == -1)
			throw Multiplex::PlexFail(); // change message
		event.events = EPOLLIN;
		event.data.fd = (*it).getFd();
		if (epoll_ctl(m_fd, EPOLL_CTL_ADD, (*it).getFd(), &event) == -1)
			throw Multiplex::PlexFail(); // change message
	}
}

void Multiplex::closePlex() { // delete everything before closing
	if (m_fd != -1)
		close(m_fd);
}