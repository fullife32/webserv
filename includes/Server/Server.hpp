/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 16:49:27 by rotrojan          #+#    #+#             */
/*   Updated: 2022/06/15 19:02:53 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

#include <arpa/inet.h>
#include <cstdlib>
#include <errno.h>
#include <iostream>
#include <iterator>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "ServerConf.hpp"
#include "Socket.hpp"

#define LISTEN_LEN 1024
#define EPOLL_TIMEOUT -1

enum e_socket_error {
	CREATE_FAIL,
	OPTION_FAIL,
	BIND_FAIL,
	LISTEN_FAIL
};

class Multiplex;

/*
	This class contains all informations needed to create and work with a
	server socket. Its file descriptor, the event associated will always be
	EPOLLIN to listen for new clients connections, its server block infos and a
	vector of all its clients.
*/
class Server : public Socket {
private:
	
	const ServerConf	&m_conf;

public:
	
	Server( int const fd, ServerConf const &conf );
	Server( Server const &other );
	~Server();

	Server &operator=( Server const &other );
private:
	
	Server();

public:
	
	static int	createSocket();
	static void	setOpts( int const fd );
	static void	bindSocket( int const fd, ServerConf const &conf );
	static void	listenSocket( int const fd );

	ServerConf const	&getConf() const;

	class SocketFail : public std::exception {
	private:

		int	m_flag;

	public:

		SocketFail( int flag ) : m_flag(flag) {}

		virtual const char	*what() const throw() {
			char const *error_msg[] = {
				"Socket creation failed",
				"Socket options failed",
				"Socket bind failed, host:port already used",
				"Socket listen failed"
			};
			return error_msg[m_flag];
		}
	};
};

#endif
