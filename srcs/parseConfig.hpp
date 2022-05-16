#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <string>

class serverLoc {
private:
	
public:
	serverLoc() {}
	~serverLoc() {}
};

class serverBlock : public serverLoc { // public ?
	std::string	_ipAddr;
	std::string	_listen;
	// std::string server_name;
	// std::vector<std::string[2]> listen;
	// std::vector<std::string> index;
	// list of serverLoc;
public:
	serverBlock() {}
	serverBlock(std::string ip, std::string port) : _ipAddr(ip), _listen(port) {}
	~serverBlock() {}

	std::string getIp() const {
		return _ipAddr;
	}
	std::string getPort() const {
		return _listen;
	}
};

#endif