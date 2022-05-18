/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:09:26 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/18 18:15:06 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"

/* --- Construct functions --- */

ServerSocket::ServerSocket( ServerBlock &block ) : m_block(block) {
	m_createSocket();
	showInfos();
	m_setOpts();
	m_bindSocket();
	m_listenSocket();
}

ServerSocket::~ServerSocket() {}

/* --- Public functions --- */

void	ServerSocket::showInfos() {
	std::cout << std::endl << "---------- Infos Socket ----------" << std::endl;
	std::cout << "sockfd: " << m_fd << std::endl;
	std::cout << "block.ip_addr: " << m_block.getIp() << std::endl;
	std::cout << "block.listen: " << m_block.getPort() << std::endl;
	std::cout << "clients:" << std::endl;
	for (std::vector<ClientSocket>::iterator it = m_clients.begin() ; it != m_clients.end(); ++it)
		std::cout << "fd: " << it->getFd() << std::endl;
	std::cout << "----------------------------------" << std::endl << std::endl;
}

int		ServerSocket::addClient() {
	sockaddr_storage	cli;
	socklen_t			size; // use it !
	int					newFd;

	size = sizeof(cli);
	newFd = accept(m_fd, (struct sockaddr *)&cli, &size);
	if (newFd == -1)
		std::cerr << "Fail adding new client" << std::endl;
	else {
		ClientSocket newClient(newFd, cli, size);
		m_clients.push_back(newClient);
		std::cout << "Successfully added client: " << newClient.getFd() << std::endl;
		// recv();
	}
	return newFd;
}

/* --- Getter functions --- */

int		ServerSocket::getFd() const {
	return m_fd;
}

int		ServerSocket::getClientFd(int fd) const {
	std::vector<ClientSocket>::const_iterator it;
	for (it = m_clients.begin(); it != m_clients.end(); ++it) {
		if ((*it).getFd() == fd)
			return (*it).getFd();
	}
	return -1;
}

/* --- Private functions --- */

void	ServerSocket::m_createSocket() {
	Socket(socket(PF_INET, SOCK_STREAM, 0));
	if (m_fd == -1)
		throw Socket::SocketFail(CREATE_FAIL);
	std::cout << "Socket successfully created with fd: " << m_fd << std::endl;
}

void	ServerSocket::m_setOpts() {
	int	yes = 1;

	if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		closeSocket();
		throw Socket::SocketFail(OPTION_FAIL);
	}
	std::cout << "Options successfully set for socket: " << m_fd << std::endl;
}

void	ServerSocket::m_bindSocket() {
	struct sockaddr_in sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_block.getPort());
	sin.sin_addr.s_addr = inet_addr(m_block.getIp());

	std::cout << "Ip address ntohs: " << ntohs(sin.sin_addr.s_addr) << std::endl;

	if (bind(m_fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
		closeSocket();
		throw Socket::SocketFail(BIND_FAIL);
	}
	std::cout << "Binding successfully made for socket: " << m_fd << std::endl;
}

void	ServerSocket::m_listenSocket() {
	if (listen(m_fd, LISTEN_LEN) == -1) {
		closeSocket();
		throw Socket::SocketFail(LISTEN_FAIL);
	}
	std::cout << "Socket now listening: " << m_fd << std::endl;
}
