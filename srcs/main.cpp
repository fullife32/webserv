#include <iostream>
#include "Server.hpp"
#include "ParseConfig.hpp"
#include "Multiplex.hpp"

int	main(int ac, char **av) {
	if (ac != 3) { // change to 2
		std::cout << "Error: ./webserv ip port" << std::endl;
		return 1;
	}

	std::vector<ServerConf>		confs; // not finished
	std::vector<Server>			servers;
	ServerConf 					test(av[1], av[2]);

	confs.push_back(test);
	for (std::vector<ServerConf>::iterator it = confs.begin(), ite = confs.end(); it != ite; ++it) {
		int fd = -1;

		try {
			fd = Server::createSocket();
			Server::setOpts(fd);
			Server::bindSocket(fd, *it);
			Server::listenSocket(fd);
			servers.push_back(Server(fd, (*it)));
		} catch (std::exception const &except) {
			if (fd != -1)
				close(fd); // free and close ???
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

	// for (;;) {
		// serverTest.showInfos();
		// if (plex.waitPlex() == -1)
			// exit(1); // don't exit
		// plex.watchEvents(serverTest);
	// }

	for (std::vector<Server>::iterator it = servers.begin(), ite = servers.end(); it != ite; ++it)
		(*it).closeSocket();
	plex.closePlex();
}
