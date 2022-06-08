/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:24 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/08 14:20:22 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLEX_HPP
# define MULTIPLEX_HPP

#include <cstring>
#include <iostream>
#include <map>
#include <cstdlib>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/signalfd.h>
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
	int			m_signal_fd;
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
	int		waitPlex();
	int		handleEvents( std::map<int, Server> &servers, std::map<int, Client> &clients );

	void	addSignalToPoll() const;
	void	handleSignal();

	void	addServersToPoll( std::map<int, Server> &servers ) const;
	void	handleServer( int i, std::map<int, Server> &servers, std::map<int, Client> &clients );

	void	addClientToPoll( Client &client ) const;
	void	handleClients( int i, std::map<int, Client> &clients );
	void	changeClientEvent( Client &client, int newEvent ) const;
	void	deleteClient( std::map<int, Client> &clients, std::map<int, Client>::iterator it );

	void	freeEvents();
	void	closePlex();

private:

	void	m_checkClientChangeEvent(std::map<int, Client>::iterator currentClient, std::map<int, Client> &clients);
	class PlexFail : public std::exception {
	public:
		PlexFail() { }
		virtual const char	*what() const throw() {
			return "Selector creation failed";
		}
	};
	class ClientFail : public std::exception {
	public:
		ClientFail() { }
		virtual const char	*what() const throw() {
			return "Client creation failed in selector";
		}
	};
};

#endif
