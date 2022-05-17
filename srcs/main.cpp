#include <iostream>
#include "ServerSocket.hpp"
#include "ParseConfig.hpp"
#include "Multiplex.hpp"

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

	ServerBlock		datas(av[1], av[2]); // not finished
	ServerSocket	serverTest(datas);
	Multiplex		plex;

	plex.addToPoll(serverTest.getFd(), serverTest.getEvent());  //loop through all server sockets
	for (;;) {
		serverTest.showInfos();
		if (plex.waitPlex() == -1)
			exit(1); // don't exit
		plex.watchEvents(serverTest);
	}
	serverTest.close();
}
