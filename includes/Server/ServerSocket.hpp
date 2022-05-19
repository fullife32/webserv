/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rotrojan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 16:49:27 by rotrojan          #+#    #+#             */
/*   Updated: 2022/05/19 18:47:53 by rotrojan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iterator>
#include <cstdlib>
#include "ParseConfig.hpp"
#include "ClientSocket.hpp"
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
class ServerSocket : public Socket {
private:
	const ServerConf					&m_conf;
	// std::vector<ClientSocket>	m_clients;

public:
	/*
		The constructor create the socket and make it listen.
	*/
	ServerSocket( int const fd, ServerConf const &conf );
	~ServerSocket();

private:
	ServerSocket();
	ServerSocket( ServerSocket const &other );
	ServerSocket &operator=( ServerSocket const &other );

public:
	void	showInfos();
	/*
		Not finished.
	*/
	// int		addClient();
	// void	closeSocket();
	/*
		Function socket needs the ip protocol, in this case IPv4 so PF_INET,
		the transport protocol, TCP so SOCK_STREAM with no flag and return a
		socket file descriptor.
	*/
	static int	createSocket();

	/* Getter functions */

	// int		getClientFd( int fd );

	/*
		This function set the socket ip/port to be reusable after finishing using
		it. It takes the socket file descriptor, at option level socket so
		SOL_SOCKET and make ip/port reusable with SO_REUSEADDR with an
		int yes = 1 to accept it or not.
	*/
	static void	setOpts( int const fd );
	/*
		This function bind the socket to an ip and a port if possible. It takes
		a structure with 3 datas, the IP family, AF_INET for IPv4, the port
		transformed to a network form with htons() and an ip transform to a
		network form too with inet_addr().
	*/
	static void	bindSocket( int const fd, ServerConf const &conf );
	/*
		This function make the socket port open for listening to accept new
		clients, LISTEN_LEN corresponding to the waiting queue max size, in our
		case, it's 1024.
	*/

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
