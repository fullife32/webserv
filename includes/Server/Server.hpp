/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 16:49:27 by rotrojan          #+#    #+#             */
/*   Updated: 2022/05/20 16:37:05 by eassouli         ###   ########.fr       */
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
#include "ParseConfig.hpp"
#include "Client.hpp"
#include "Socket.hpp"

#define LISTEN_LEN 1024
#define EPOLL_TIMEOUT -1

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

private:
	Server();
	Server &operator=( Server const &other );

public:
	void	showInfos(); // debug

	static int	createSocket();
	static void	setOpts( int const fd );
	static void	bindSocket( int const fd, ServerConf const &conf ); // Careful 255.255.255.255
	static void	listenSocket( int const fd );

	class SocketFail : public std::exception {
		int	m_flag;
		// char *error_msg[] = {
		// 	"Socket creation failed",
		// 	"Socket options failed",
		// 	"Socket bind failed",
		// 	"Socket listen failed"
		// };

	public:
		SocketFail( int flag ) : m_flag(flag) {}
		virtual const char	*what() const throw() {
			return "Socket failed";
		}
	};
};

#endif
