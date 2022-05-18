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
	ServerBlock					&m_block;
	std::vector<ClientSocket>	m_clients;

public:
	/*
		The constructor create the socket and make it listen.
	*/
	ServerSocket( ServerBlock &block );
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
	int		addClient();
	void	closeSocket();

	/* Getter functions */

	int		getFd() const;
	int		getClientFd( int fd ) const;

private:
	/*
		Function socket needs the ip protocol, in this case IPv4 so PF_INET,
		the transport protocol, TCP so SOCK_STREAM with no flag and return a
		socket file descriptor.
	*/
	void	m_createSocket();
	/*
		This function set the socket ip/port to be reusable after finishing using
		it. It takes the socket file descriptor, at option level socket so
		SOL_SOCKET and make ip/port reusable with SO_REUSEADDR with an
		int yes = 1 to accept it or not.
	*/
	void	m_setOpts();
	/*
		This function bind the socket to an ip and a port if possible. It takes
		a structure with 3 datas, the IP family, AF_INET for IPv4, the port
		transformed to a network form with htons() and an ip transform to a
		network form too with inet_addr().
	*/
	void	m_bindSocket();
	/*
		This function make the socket port open for listening to accept new
		clients, LISTEN_LEN corresponding to the waiting queue max size, in our
		case, it's 1024.
	*/
	void	m_listenSocket();
};

#endif
