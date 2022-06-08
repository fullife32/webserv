/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:31 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/07 16:41:19 by lvirgini         ###   ########.fr       */
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

public:
	Server				&m_server;

private:

	bool				m_toRemove; 		// ?
	bool				m_toChangeEvent; 	// ?
	sockaddr_storage 	m_cli;
	socklen_t			m_size;
	RequestHTTP			m_request;
	ResponseHTTP		m_response;
	char				m_buffer[MESSAGE_BUFFER_SIZE + 1];

	Client();
	Client &operator=( Client const &other );

public:
	Client( int fd, sockaddr_storage cli, socklen_t size, Server &server );
	Client( Client const &other );
	~Client();


	static Client	acceptClient( int fdServer, Server &server );

	bool			getToRemove() const;
	bool			getToChangeEvent() const;

	void			setToRemove(); // end send() or recv == -1;
	void			setToChangeEvent(); // end recv()

	void			receive_data();
	void			send_data();


	class ClientFail : public std::exception {
	public:
		ClientFail() { }
		virtual const char	*what() const throw() {
			return "Client create failed";
		}
	};
};

#endif
