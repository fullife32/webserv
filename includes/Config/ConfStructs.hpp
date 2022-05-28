/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfStructs.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:32:14 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/28 16:08:23 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_STRUCTS_HPP
# define CONF_STRUCTS_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>

struct s_location {
	std::map<size_t, std::string>		error_page; // error path
	size_t								client_max_body_size;
	std::vector<std::string>			method; // by default GET, POST and DELETE
	std::pair<size_t, std::string>		redirect;
	std::string							root;
	bool								autoindex; // default off/false
	std::vector<std::string>			index; // default index.html ?
	std::map<std::string, std::string>	cgi; // default ./cgi-bin ?
	std::string							upload_pass;
};

struct s_server { // flag is set to some directives
	std::pair<std::string, size_t>		listen; // if no port stop // if no address bind 0.0.0.0 or 127.0.0.1 // if localhost replace with 127.0.0.1
	std::vector<std::string>			server_name;
	std::vector<struct s_location>		location;

	std::map<size_t, std::string>		error_page; // error path
	size_t								client_max_body_size;
	std::pair<size_t, std::string>		redirect;
	std::string							root;
	bool								autoindex; // default off/false
	std::vector<std::string>			index; // default index.html ?
	std::string							upload_pass;
};

#endif
