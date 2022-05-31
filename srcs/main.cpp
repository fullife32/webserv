/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:41 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/31 16:04:18 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include "Server.hpp"
#include "ServerConf.hpp"
#include "Multiplex.hpp"
#include "Client.hpp"
#include "utils.hpp"

int	main(int ac, char **av) {
	if (ac != 2) { // if only one arg go to default path
		std::cerr << "Error: ./webserv path_to_configuration_file" << std::endl;
		return 1;
	}

	std::vector<ServerConf>	confs;

	if (ServerConf::startParse(av[1], confs) == 1)
		return 1;

	std::map<int, Server>	servers;

	for (std::vector<ServerConf>::iterator it = confs.begin(), ite = confs.end(); it != ite; ++it) {
		int fd = -1;

		try {
			fd = Server::createSocket();
			Server::setOpts(fd);
			Server::bindSocket(fd, *it);
			Server::listenSocket(fd);
			servers.insert(std::make_pair(fd, Server(fd, (*it))));
			// Debug
			Server	tmp = servers.rbegin()->second;
			std::cout << tmp.getFd() << ": server started on " << tmp.getConf().getIp() << ":" << tmp.getConf().getPort() << std::endl;
			//
		} catch (std::exception const &except) {
			if (fd != -1)
				close(fd);
			for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
				it->second.closeSocket();
			std::cerr << except.what() << std::endl;
			return 1;
		}
	}

	Multiplex	plex;

	try {
		plex.createPlex();
		plex.addServersToPoll(servers);
	} catch (std::exception const &except) {
		for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
			it->second.closeSocket();
		std::cerr << except.what() << std::endl;
		return 1;
	}

	std::map<int, Client>	clients;

	for (;;) {
		if (plex.waitPlex() == -1)
			break;
		plex.handleEvents(servers, clients);
	}

	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
		it->second.closeSocket();
	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
		it->second.closeSocket();
}
