/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:25:48 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/24 18:20:59 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf(std::string ip, std::string port)
: _ipAddr(ip), _listen(port) {}

ServerConf::~ServerConf() { }

void	ServerConf::openFile( std::string filePath, std::ifstream &ifs ) {
	ifs.open(filePath.c_str(), std::ifstream::in);
	if (ifs.eof()) {
		throw ServerConf::ConfFail(); // Empty file
	}
	else if (!ifs.good()) {
		throw ServerConf::ConfFail(); // Impossible to open the file
	}
}
