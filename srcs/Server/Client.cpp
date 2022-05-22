/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:21:11 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/20 19:13:00 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( int fd, sockaddr_storage cli, socklen_t size, Server &server )
: Socket(fd), m_cli(cli), m_size(size), m_server(server) { }

Client::Client( Client const &other )
: Socket(other.m_fd), m_cli(other.m_cli), m_size(other.m_size), m_server(other.m_server) { }

Client::~Client() { }

Client	Client::acceptClient( int fdServer, Server &server ) {
	sockaddr_storage	cli;
	socklen_t			size;
	int					fdClient;
	
	size = sizeof(cli);
	fdClient = accept(fdServer, (struct sockaddr *)&cli, &size);
	if (fdServer == -1 || fdServer == 0)
		throw Client::ClientFail();
	return Client(fdClient, cli, size, server);
}