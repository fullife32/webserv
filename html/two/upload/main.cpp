/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:34:41 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/13 17:38:50 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <signal.h>
#include "Server.hpp"
#include "ServerConf.hpp"
#include "Multiplex.hpp"
#include "Client.hpp"
#include "utils.hpp"

int	main(int ac, char **av) {


// 	char buf[256];

// 	std::ifstream	f("html/three/music/images/background.jpg", std::fstream::binary );

// 	// std::stringstream s;

// 	// s << f;
// 	// std::cout << s.str() << std::endl;
// 		// if (f.good())
// 			// std::cout << "coucou" << std::endl;
// 			send(1, "coucou", 6, 0);
// 		while (f.good())
// 		{
// 			memset(buf, 0, 256);
// 			f.read(buf, 256);
// 			write(1, buf, 256);

// 			// str.append(buf);
// 			std::cout << buf;
// 		}
// 		// send(1, str.data(), str.size(),0);
// 		write(1, "\0", 1);
		
// // 
// return (0);


// char buffer [256] = {0} ;
//   FILE * pFile = tmpfile ();

// /// CLIENT RESPONSE

// 	fputs ("buffer client send\n", pFile); // puts body recv() from client


// 	fseek ( pFile , 3 , SEEK_SET ); // fseek if keeping header and move to end Header size;
// 	fputs ("POUET\n", pFile); // puts body recv() from client
	
	
	
// 	rewind(pFile); // go to the begining of file


// //// CGI fileno

// 	int fd = fileno(pFile);

// 	int fd2 = fileno(pFile2);

// 	std::cout << "fd: " << fd << std::endl;


// ////// Response to Client

//   while (!feof(pFile)) {
//     if (fgets (buffer,256,pFile) == NULL) break;
// 	std::cout << buffer << std::endl;
//   }

// std::cout << read(fd, buffer, 256) << std::endl;
// std::cout << fclose (pFile) << std::endl;
// std::cout << close (fd) << std::endl;


// 	std::cout << buffer << std::endl;



//   return 0;

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

	// ServerConf::getConfTest(confs); // TODO DEBUG

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
		if (plex.handleEvents(servers, clients) == -1)
			break;
	}

	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
		it->second.closeSocket();
	for (std::map<int, Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
		it->second.closeSocket();
}