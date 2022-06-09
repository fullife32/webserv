/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:21:11 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/09 18:41:48 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ErrorMessage.hpp"

Client::Client( int fd, sockaddr_storage cli, socklen_t size, Server &server )
: Socket(fd),  m_server(server), m_toRemove(false), m_toChangeEvent(false), m_cli(cli), m_size(size), m_response(&(m_server.getConf())) {
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
	size = recv(m_fd, m_buffer, MESSAGE_BUFFER_SIZE, 0); // TODO; recv first == 0 le client s est deconnecte

	if (size == -1)
	{
		setToChangeEvent();
		m_response.buildError(STATUS_INTERNAL_SERVER_ERROR, S_STATUS_INTERNAL_SERVER_ERROR, m_response.get_url());
		return ;
	}
	m_request.append(m_buffer);
	if (size == 0 && m_request.empty())
		setToRemove();
	else if (size == 0 || size < MESSAGE_BUFFER_SIZE)
	{
		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);
		try {
			m_request.buildRequest();
			// m_request.debug_print();
			m_response.buildResponse(m_request);
		}
		catch (MessageErrorException & e) {
			m_response.buildError(e.getError(), e.getMappedError(), e.getUrl());
		}
		catch (std::exception & e) {
			std::cerr << e.what() << std::endl;
			setToRemove();
		}
		setToChangeEvent();
	}	
}


void		Client::send_data() {

	static size_t	bufferSize = 0;
	static size_t	sendSize = 0;

	if (sendSize == bufferSize)
	{
		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);
		bufferSize = m_response.getNextChunk(m_buffer);
	}
	if (bufferSize == 0) {
		setToRemove();
		return;
	}
	sendSize = send(m_fd, m_buffer, bufferSize, MSG_NOSIGNAL);
	if (sendSize == (size_t)-1 || sendSize == 0 || bufferSize < MESSAGE_BUFFER_SIZE)
		setToRemove(); // TODO: what to do ? 


// }

// void	Client::receive_data_from_CGI()
// {
// 	getline()
// 	if (m_buffer.isEmpty())
// 		recv(fd_CGI);
// 		m_cgi is ready = true;
// }

// void	Client::send_data_CGI() {


// 	if (m_response.CGIhasheaderToSend)
// 		send_data()
		
// 	if (m_CGIisready == true )
// 	{
// 		static size_t	bufferSize = 0;

// 		bufferSize = m_response.getNextChunk_CGI(m_buffer); // HEADER
// 		send(m_fd, m_buffer);
// 		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);
// 		m_cgi is ready = false;
// 	}

}