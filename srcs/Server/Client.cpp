/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:21:11 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/04 14:02:13 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ErrorMessage.hpp"

Client::Client( int fd, sockaddr_storage cli, socklen_t size, Server &server )
: Socket(fd), m_toRemove(false), m_toChangeEvent(false), m_cli(cli), m_size(size), m_server(server), m_request(), m_response(&m_server, m_buffer) {
 }

Client::Client( Client const &other )
: Socket(other.m_fd), m_cli(other.m_cli), m_size(other.m_size), m_server(other.m_server), m_request(other.m_request), m_response(other.m_response) { 
}

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

bool	Client::getToRemove() const {
	return m_toRemove;
}

bool	Client::getToChangeEvent() const {
	return m_toChangeEvent;
}

void			Client::setToRemove() {
	if (m_toRemove)
		m_toRemove = false;
	else
		m_toRemove = true;
}
void			Client::setToChangeEvent() {
	if (m_toChangeEvent)
		m_toChangeEvent = false;
	else
		m_toChangeEvent = true;
}


void		Client::receive_data() {

	int	size;

	memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);
	size = recv(m_fd, m_buffer, MESSAGE_BUFFER_SIZE, 0);
	if (size == -1)
	{
		setToChangeEvent();
		m_response.buildError(STATUS_INTERNAL_SERVER_ERROR, S_STATUS_INTERNAL_SERVER_ERROR);
	}
	m_request.append(m_buffer);
	if (size == 0)
	{
		m_response.buildResponse(m_request);
		setToChangeEvent();
	}	
}


void		Client::send_data() {

	size_t	size;

	size = m_response.getNextChunk();

	if (size == 0)
		setToChangeEvent();
	else if (send(m_fd, m_buffer, size, 0) == -1)
		setToRemove();
}


