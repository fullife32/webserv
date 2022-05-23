/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfStructs.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:32:14 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/23 16:34:21 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_STRUCTS_HPP
# define CONF_STRUCTS_HPP

#include <string>
#include <vector>
#include <map>

struct s_location {
	int									body_size;
	std::string							address; // if no address bind 0.0.0.0
	std::string							port; // if no port bind 80
	std::vector<std::string>			server_names;
	std::vector<std::string>			allowed_methods;
	std::string							root;
	std::string							auth[2];
	std::map<int, std::string>			error_pages; // error path
	std::string							upload;
	bool								autoindex; // default off/false
	std::vector<std::string>			indexes; // default index.html ?
	std::map<std::string, std::string>	cgis;
	std::string							cgi_bin; // default ./cgi-bin
};

struct s_server {
	size_t								body_size;
	std::string							address; // if no address bind 0.0.0.0
	std::string							port; // if no port bind 80
	std::vector<std::string>			server_names;
	std::string							root;
	std::map<int, std::string>			error_pages; // error path
	// std::string							upload;
	bool								autoindex; // default off/false
	std::vector<std::string>			indexes; // default index.html ?
	std::map<std::string, std::string>	cgis;
	std::string							cgi_bin; // default ./cgi-bin
	std::vector<struct s_location>		locations;
};

#endif
