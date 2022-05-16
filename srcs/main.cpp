#include "Webserv.hpp"


int	plexSocket(serverSocket socket);

int	main(int ac, char **av) {
	int	epfd; //don't forget to close it

	if (ac != 3) {
		std::cout << "./webserv ip port" << std::endl;
		return 1;
	}
	serverBlock		datas(av[1], av[2]);
	serverSocket	serverTest(datas);

	epfd = plexSocket(serverTest);
	while(1) {
		int ready;
		epoll_event waitList;
		waitList.events = EPOLLIN;
		ready = epoll_wait(epfd, &waitList, 10, -1);
		if (ready == -1) {
			std::cout << "epoll_wait failed" << std::endl;
			break;
		}
		if (ready > 1)
		{
			std::cout << "Finished waiting" << std::endl;
			std::cout << serverTest.getEvent() << std::endl;

		}	
	}
	serverTest.close();
}
