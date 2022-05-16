#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <string>

/*
	Those classes will have all infos of one server block received from the
	config file.
*/
class LocBlock {
private:
	
public:
	LocBlock() {}
	~LocBlock() {}
	// LocBlock( LocBlock const &other );
	// LocBlock &operator=( LocBlock const &other );
};

class ServerBlock {
	std::string	_ipAddr;
	std::string	_listen;
	// std::string server_name;
	// std::vector<std::string[2]> listen;
	// std::vector<std::string> index;
	// list of LocBlock;
public:
	ServerBlock() {}
	ServerBlock(std::string ip, std::string port) : _ipAddr(ip), _listen(port) {}
	~ServerBlock() {}
	// ServerBlock( ServerBlock const &other );
	// ServerBlock &operator=( ServerBlock const &other );

	const char *getIp() const {
		return _ipAddr.c_str();
	}
	int	getPort() const {
		return atoi(_listen.c_str());
	}
};

#endif