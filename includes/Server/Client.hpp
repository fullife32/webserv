/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:31 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/02 16:38:34 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "Server.hpp"
#include "MessageHTTP.hpp"

/*
	This class contains all informations needed to create and work with a
	client socket, to send and recv packages. It basically contains its fd, the
	client datas received with accept() and the events waited.
*/
class Client : public Socket {
private:

	bool				m_toRemove; // ?
	bool				m_toChangeEvent; // ?
	sockaddr_storage 	m_cli;
	socklen_t			m_size;
	Server				&m_server;
	WS::ResponseHTTP	m_response;
	WS::RequestHTTP		m_request;

public:

	Client( int fd, sockaddr_storage cli, socklen_t size, Server &server );
	Client( Client const &other );
	~Client();

private:

	Client();s
	{
		response.setServer(m_server);
	}


	Client &operator=( Client const &other );

public:

	static Client	acceptClient( int fdServer, Server &server );

	bool			getToRemove() const;
	bool			getToChangeEvent() const;

	void			setToRemove();
	void			setToChangeEvent();

	void			recv(char *buffer)
	{
		request.append(buffer);
	}

	char *			send()
	{
		return response.getNextChunk(MESSAGE_BUFFER_SIZE)
	}


	class ClientFail : public std::exception {
	public:
		ClientFail() { }
		virtual const char	*what() const throw() {
			return "Client create failed";
		}
	};
};

#endif
