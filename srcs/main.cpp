/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:41 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/22 14:51:49 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include "Server.hpp"
#include "ParseConfig.hpp"
#include "Multiplex.hpp"
#include "Client.hpp"

int	main(int ac, char **av) {
	if (ac != 3) { // change to 2
		std::cout << "Error: ./webserv ip port" << std::endl;
		return 1;
	}

	std::vector<ServerConf>		confs; // not finished
	std::map<int, Server>		servers;
	std::map<int, Client>		clients;
	ServerConf 					test(av[1], av[2]);

	confs.push_back(test);
	for (std::vector<ServerConf>::iterator it = confs.begin(), ite = confs.end(); it != ite; ++it) {
		int fd = -1;

		try {
			fd = Server::createSocket();
			Server::setOpts(fd);
			Server::bindSocket(fd, *it);
			Server::listenSocket(fd);
			servers.insert(std::make_pair(fd, Server(fd, (*it))));
		} catch (std::exception const &except) {
			if (fd != -1)
				close(fd); // free, close and exit ???
			std::cerr << except.what() << std::endl;
		}
	}

	Multiplex	plex;

	try {
		plex.createPlex();
		plex.addServersToPoll(servers);  //loop through all server sockets
	} catch (std::exception const &except) {
		plex.closePlex();
		std::cerr << except.what() << std::endl; // free and close ???
	}

	for (;;) {
		// serverTest.showInfos();
		if (plex.waitPlex() == -1)
			break;
		plex.handleEvents(servers, clients);
	}

	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
		it->second.closeSocket();
	plex.closePlex();
}
