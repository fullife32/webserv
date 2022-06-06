/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfStructs.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:32:14 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/05 21:44:00 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_STRUCTS_HPP
# define CONF_STRUCTS_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>

struct s_base {
	std::map<size_t, std::string>			error_page;
	size_t									client_max_body_size;
	std::pair<size_t, std::string>			redirect;
	std::string								root;
	bool									autoindex;
	std::string								index;
	std::string								upload_pass;
};

struct s_location : public s_base {
	std::vector<std::string>				method;
	std::map<std::string, std::string>		cgi;
};

struct s_server : public s_base {
	std::pair<std::string, size_t>			listen;
	std::vector<std::string>				server_name;
	std::map<std::string, struct s_location>location;
};

#endif
