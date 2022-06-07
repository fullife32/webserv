/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:09:26 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/20 15:03:49 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* --- Construct functions --- */

Server::Server( int const fd, ServerConf const &conf )
: Socket(fd), m_conf(conf) {}

Server::Server( Server const &other )
: Socket(other.m_fd), m_conf(other.m_conf) {}

Server::~Server() {}

// Server &	Server::operator=( Server const &other ) {
// 	if (this != &other)
// 	{
// 		m_conf = other.m_conf;
// 	}
// }


/* --- Public functions --- */

int	Server::createSocket() {
	int fd;

	fd = socket(PF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		throw Server::SocketFail(CREATE_FAIL);
	return fd;
}

void	Server::setOpts( int const fd ) {
	int	yes = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw Server::SocketFail(OPTION_FAIL);
}

void	Server::bindSocket( int const fd, ServerConf const &conf ) {
	struct sockaddr_in sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(conf.getPort());
	sin.sin_addr.s_addr = inet_addr(conf.getIp());

	if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
		throw Server::SocketFail(BIND_FAIL);
}

void	Server::listenSocket( int const fd ) {
	if (listen(fd, LISTEN_LEN) == -1)
		throw Server::SocketFail(LISTEN_FAIL);
}

ServerConf const	&Server::getConf() const {
	return m_conf;
}
