#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <sys/epoll.h>
#include <cstdlib>

#define CREATE_FAIL 0
#define OPTION_FAIL 1
#define BIND_FAIL 2
#define LISTEN_FAIL 3

/*
	This class serve as an assembly of all the basic infos of a socket,
	compatible with a server and a client.
*/
class Socket {
protected:
	const int	m_fd;

public:
	Socket(int fd) : m_fd(fd) {}
	~Socket() {}

private:
	Socket( Socket const &other );
	Socket &operator=( Socket const &other );

public:
	// int		getFd() const {
	// 	return m_fd;
	// }
	class SocketFail : public std::exception {
		int	m_flag;
		// char *error_msg[] = {
		// 	"Socket creation failed",
		// 	"Socket options failed",
		// 	"Socket bind failed",
		// 	"Socket listen failed"
		// };

		public:
			SocketFail(int flag) : m_flag(flag) {}
			virtual const char*	what() const throw() {
				return "Socket failed";
			}
	};

	void	closeSocket() {
		if (m_fd != -1)
			close(m_fd);
	}
};

#endif
