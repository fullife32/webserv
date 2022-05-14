#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

// Move all of this ------------
typedef struct s_serverBlock {
	std::string			ipAddr;
	std::string			listen;
	// std::string server_name;
	// std::vector<std::string[2]> listen;
	// std::vector<std::string> index;
	// list of serverLoc;
}				t_serverBlock;

typedef struct s_serverLoc {
	/* data */
}				t_serverLoc;
// -----------------------------

class serverSocket {
private:
	int				_sockfd;
	int				_yes;
	t_serverBlock	_block;

public:
	serverSocket(std::string ip, std::string port) {
		_block.ipAddr = ip;
		_block.listen = port;
		_yes = 1;
		_createSocket();
		_setOpts();
		_bindSocket();
		_listenSocket();
		showInfos();
	}
	~serverSocket() {
		std::cout << "Closing socket: " << _sockfd << std::endl;
		if (_sockfd != -1)
			close(_sockfd);
	}
	// serverSocket( serverSocket const &other );
	// serverSocket &operator=( serverSocket const &other );
	void	showInfos() {
		std::cout << std::endl << "---------- Infos Socket ----------" << std::endl;
		std::cout << "sockfd: " << _sockfd << std::endl;
		std::cout << "yes: " << _yes << std::endl;
		std::cout << "block.ip_addr: " << _block.ipAddr << std::endl;
		std::cout << "block.listen: " << _block.listen << std::endl;
		std::cout << "----------------------------------" << std::endl << std::endl;
	}

private:
	void	_createSocket() {
		this->_sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if (this->_sockfd == -1) {
			std::cout << "Fail creating socket" << std::endl;
			exit(1);
		}
		std::cout << "Socket successfully created with fd: " << this->_sockfd << std::endl;
	}
	void	_setOpts() {
		int	ret;

		if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &this->_yes, sizeof(int)) == -1) {
			std::cout << "Fail setting options for socket: " << _sockfd << std::endl; // throw exceptions instead
			exit(1);
		}
		std::cout << "Options successfully set for socket: " << this->_sockfd << std::endl;
	}
	void	_bindSocket() {
		struct sockaddr_in sin;

		sin.sin_family = AF_INET;
		sin.sin_port = htons(atoi(_block.listen.c_str()));
		sin.sin_addr.s_addr = inet_addr(_block.ipAddr.c_str());
		if (bind(this->_sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
			std::cout << "Fail binding with the socket: " << _sockfd << std::endl;
			exit(1);
		}
		std::cout << "Binding successfully made for socket: " << this->_sockfd << std::endl;
	}
	void	_listenSocket() {
		if (listen(_sockfd, 5) == -1) {
			std::cout << "Fail listening socket: " << _sockfd << std::endl;
			exit(1);
		}
		std::cout << "Socket now listening: " << this->_sockfd << std::endl;
	}
};

#endif