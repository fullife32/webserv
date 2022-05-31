/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 11:14:39 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/23 16:20:37 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <string>
#include <cstdlib>

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

class ServerConf {
	std::string	_ipAddr;
	std::string	_listen;
	// std::string server_name;
	// std::vector<std::string[2]> listen;
	// std::vector<std::string> index;
	// list of LocBlock;
public:
	ServerConf() {}
	ServerConf(std::string ip, std::string port) : _ipAddr(ip), _listen(port) {}
	~ServerConf() {}
	// ServerConf( ServerConf const &other );
	// ServerConf &operator=( ServerConf const &other );

	const char *getIp() const {
		return _ipAddr.c_str();
	}
	int	getPort() const {
		return atoi(_listen.c_str());
	}
};

#endif
