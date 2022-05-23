/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:25:48 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/23 18:19:45 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf() { }

ServerConf::ServerConf(std::string ip, std::string port) : _ipAddr(ip), _listen(port) {}

ServerConf::~ServerConf() { }

int	ServerConf::checkFile( std::string filePath ) {
	int	fd;

	// check extension
	throw ServerConf::ConfFail();
	return fd;
}
