/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:41 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/02 19:31:07 by eassouli         ###   ########.fr       */
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
	std::string	path = DEFAULT_CONFIG_PATH;

	if (ac == 2)
		path = av[1];
	else if (ac > 2) {
		std::cerr << "Error: ./webserv [path_to_configuration_file]" << std::endl;
		return 1;
	}

	std::vector<ServerConf>	confs;

	if (ServerConf::startParse(path, confs) == 1)
		return 1;
	std::cout << "Method allowed ? " << confs.at(0).isMethodAllowed("vo", "/", 1) << std::endl;
	std::string url;
	std::cout << "Redirecting ? " << confs.at(0).isRedirecting("vo", "/", url) << " > " << url << std::endl;
	std::cout << "Location \"\" ? " << confs.at(0).getLocationPath("vo", "") << std::endl;
	std::cout << "Location / ? " << confs.at(0).getLocationPath("vo", "/") << std::endl;
	std::cout << "Location bidule ? " << confs.at(0).getLocationPath("vo", "/bidule") << std::endl;
	std::cout << "Error page 404 ? " << confs.at(0).getErrorPage("vo", "/", 404) << std::endl;
	std::cout << "Error page non exist ? " << confs.at(0).getErrorPage("vo", "/", 400) << std::endl;
	std::cout << std::endl;

	std::map<int, Server>	servers;

	for (std::vector<ServerConf>::iterator it = confs.begin(), ite = confs.end(); it != ite; ++it) {
		int fd = -1;

		try {
			fd = Server::createSocket();
			Server::setOpts(fd);
			Server::bindSocket(fd, *it);
			Server::listenSocket(fd);
			servers.insert(std::make_pair(fd, Server(fd, (*it))));
			// TODO Debug
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
