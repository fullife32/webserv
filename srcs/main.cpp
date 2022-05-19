#include <iostream>
#include "ServerSocket.hpp"
#include "ParseConfig.hpp"
//#include "Multiplex.hpp"

/*
	Starting by gathering config file infos, then creating each socket
	associated with a server block and the multiplexing everything (adding an
	event list and waiting for all of them at the same time).
*/
int	main(int ac, char **av) {
	if (ac != 3) {
		std::cout << "./webserv ip port" << std::endl;
		return 1;
	}

	std::vector<ServerConf>		confs; // not finished
	std::vector<ServerSocket>	servers;
	ServerConf test(av[1], av[2]);

	confs.push_back(test);
	for (std::vector<ServerConf>::iterator it = confs.begin(), ite = confs.end(); it != ite; ++it) {
		int fd = -1;
		try {
			fd = ServerSocket::createSocket();
			ServerSocket::setOpts(fd);
			std::cout << inet_addr((*it).getIp()) << std::endl;
			ServerSocket::bindSocket(fd, *it);
			ServerSocket::listenSocket(fd);
		} catch (std::exception const &except) {
			if (fd != -1)
				close(fd);
			std::cerr << except.what() << std::endl;
		}
	}
	// Multiplex		plex;

	// plex.addToPoll(serverTest.getFd());  //loop through all server sockets
	// for (;;) {
		// serverTest.showInfos();
		// if (plex.waitPlex() == -1)
			// exit(1); // don't exit
		// plex.watchEvents(serverTest);
	// }
	// serverTest.closeSocket();
}
