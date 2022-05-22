/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:27 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/20 19:34:28 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <sys/epoll.h>
#include <cstdlib>
#include <unistd.h>

#define CREATE_FAIL 0
#define OPTION_FAIL 1
#define BIND_FAIL 2
#define LISTEN_FAIL 3

/*
	This class serve as an assembly of all the basic infos of a socket,
	compatible with a server and a client.
*/
class Socket {
protected:
	const int	m_fd;

public:
	Socket( int fd );
	~Socket();

private:
	Socket( Socket const &other );
	Socket &operator=( Socket const &other );

public:
	void	closeSocket( void );
	int		getFd( void ) const;

};

#endif
