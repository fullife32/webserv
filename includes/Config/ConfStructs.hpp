/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfStructs.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:32:14 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/31 17:03:10 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_STRUCTS_HPP
# define CONF_STRUCTS_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>

struct s_base {
	std::map<size_t, std::string>			error_page; // error path
	size_t									client_max_body_size;
	std::pair<size_t, std::string>			redirect;
	std::string								root;
	bool									autoindex; // default off/false
	std::vector<std::string>				index; // default index.html ?
	std::string								upload_pass;
};

struct s_location : public s_base {
	std::vector<std::string>			method; // by default GET, POST and DELETE
	std::map<std::string, std::string>	cgi; // default ./cgi-bin ?
};

struct s_server : public s_base { // flag is set to some directives
	std::pair<std::string, size_t>			listen; // if no port stop // if no address bind 0.0.0.0 or 127.0.0.1 // if localhost replace with 127.0.0.1
	std::vector<std::string>				server_name;
	std::map<std::string, struct s_location>location;
};

#endif
