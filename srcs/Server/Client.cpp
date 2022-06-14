/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:21:11 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/14 20:01:24 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ErrorMessage.hpp"

Client::Client( int fd, sockaddr_storage cli, socklen_t size, Server &server )
: Socket(fd),  m_server(server), m_toRemove(false), m_toChangeEvent(false), m_cli(cli), m_size(size), m_request(&m_server.getConf()), m_response(&m_server.getConf()) {
	memset(m_buffer, 0, MESSAGE_BUFFER_SIZE + 1);
 }

Client::Client( Client const &other )
: Socket(other.m_fd),  m_server(other.m_server), m_toRemove(false), m_toChangeEvent(false), m_cli(other.m_cli), m_size(other.m_size), m_request(other.m_request), m_response(other.m_response) { 
	memset(m_buffer, 0, MESSAGE_BUFFER_SIZE + 1);
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
		m_response.buildError(STATUS_INTERNAL_SERVER_ERROR, S_STATUS_INTERNAL_SERVER_ERROR, m_response.get_url());
		return ;
	}
	if (size == 0 && m_request.empty())
	{
		setToRemove();
		return ;
	}
	// TRY BUILD HEADER
	try
	{
		m_request.append(m_buffer, m_buffer, size); // TODO add size et char *
	}
	catch (MessageErrorException & e) {
		m_response.buildError(e.getError(), e.getMappedError(), e.getUrl());
		setToChangeEvent();
		return ;
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		setToRemove();
		return ;
	}
	
	// IF END OR LAST BUFFER RECV
	if (size == 0 || size < MESSAGE_BUFFER_SIZE)
	{
		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);
		setToChangeEvent();
		try {
			m_request.debug_print();
			m_response.buildResponse(m_request);
			m_response.debug_print(); // TODO DEBUG
		}
		catch (MessageErrorException & e) {
			m_response.buildError(e.getError(), e.getMappedError(), e.getUrl());
		}
		catch (std::exception & e) {
			std::cerr << e.what() << std::endl;
			setToRemove();
		}
	}	
}


void		Client::send_data() {

	size_t	bufferSize = 0;
	size_t	sendSize = 0;

	memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);
	bufferSize = m_response.getNextChunk(m_buffer);
	if (bufferSize == 0 && m_response.need_to_read() == false) 
	{
		setToRemove();
		return;
	}
	sendSize = send(m_fd, m_buffer, bufferSize, MSG_NOSIGNAL);
	memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);
	if (sendSize == (size_t)-1 || sendSize == 0 || ((bufferSize < MESSAGE_BUFFER_SIZE) && m_response.need_to_read() == false))
		setToRemove();
}
