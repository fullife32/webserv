/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:24 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/23 14:38:40 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLEX_HPP
# define MULTIPLEX_HPP

#include <cstring>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <sys/epoll.h>
#include "Client.hpp"
#include "Server.hpp"

#define MAXEVENTS 8

/*
	This class create an epoll list and have functions to add a new socket to
	it, modify events associated with socket and waiting all the events in the
	list to happen.
*/
class Multiplex {
private:
	int			m_fd;
	int			m_nbReady;
	epoll_event	*m_events;

public:
	Multiplex();
	~Multiplex();

private:
	Multiplex( Multiplex const &other );
	Multiplex &operator=( Multiplex const &other );
	
public:
	void	createPlex();
	void	addServersToPoll( std::map<int, Server> &servers ) const;
	void	addClientToPoll( Client &client ) const;
	int		waitPlex();
	void	handleEvents( std::map<int, Server> &servers, std::map<int, Client> &clients );
	void	handleServer( int i, std::map<int, Server> &servers, std::map<int, Client> &clients );
	void	handleClients( int i, std::map<int, Client> &clients );
	void	freeEvents();
	void	closePlex();

	class PlexFail : public std::exception {
	public:
		PlexFail() { }
		virtual const char	*what() const throw() {
			return "Plex create failed";
		}
	};
};

#endif
