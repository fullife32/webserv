/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 11:14:39 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/19 15:16:25 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <string>
#include <cstdlib>
#include <vector>
#include <map>

#define AUTH_LOGIN 0
#define AUTH_PASS 1

/*
	Those classes will have all infos of one server block received from the
	config file.
*/

struct s_location {

};

struct s_server {
	std::string							workers;
	std::string							address; // if no address bind 0.0.0.0
	std::string							port; // if no port bind 80
	std::vector<std::string>			server_names;
	std::string							root;
	std::string							auth[2];
	std::map<int, std::string>			error_pages; // error path
	std::string							upload;
	bool								autoindex; // default off/false
	std::vector<std::string>			indexes; // default index.html ?
	std::map<std::string, std::string>	extensions;
	std::string							cgiFolder;
	std::vector<struct s_location>		locations;
};

// when get one info check if exist or if default possible like


class ParseConfig {
	std::string	_ipAddr;
	std::string	_listen;
	// std::string server_name;
	// std::vector<std::string[2]> listen;
	// std::vector<std::string> index;
	ParseConfig() {}
	ParseConfig( ParseConfig const &other );
	ParseConfig &operator=( ParseConfig const &other );
public:
	ParseConfig( std::string configPath ) {}
	~ParseConfig() {}
};

#endif
