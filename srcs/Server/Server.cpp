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

/* --- Public functions --- */

void	Server::showInfos() {
	std::cout << std::endl << "---------- Infos Socket ----------" << std::endl;
	std::cout << "sockfd: " << m_fd << std::endl;
	std::cout << "conf.ip_addr: " << m_conf.getIp() << std::endl;
	std::cout << "conf.listen: " << m_conf.getPort() << std::endl;
	// std::cout << "clients:" << std::endl;
	// for (std::vector<Client>::iterator it = m_clients.begin() ; it != m_clients.end(); ++it)
		// std::cout << "fd: " << it->getFd() << std::endl;
	// std::cout << "----------------------------------" << std::endl << std::endl;
}

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

// int		Server::addClient() {
	// sockaddr_storage	cli;
	// socklen_t			size; // use it !
	// int					newFd;

	// size = sizeof(cli);
	// newFd = accept(m_fd, (struct sockaddr *)&cli, &size);
	// if (newFd == -1)
		// std::cerr << "Fail adding new client" << std::endl;
	// else {
		// Client newClient(newFd, cli, size);
		// m_clients.push_back(newClient);
		// std::cout << "Successfully added client: " << newClient.getFd() << std::endl;
		// // recv();
	// }
	// return newFd;
// }

// int		Server::getClientFd( int const fd ) const {
	// std::vector<Client>::const_iterator it;
	// for (it = m_clients.begin(); it != m_clients.end(); ++it) {
		// if ((*it).getFd() == fd)
			// return (*it).getFd();
	// }
	// return -1;
// }