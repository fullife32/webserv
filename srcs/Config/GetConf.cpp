/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetConf.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 17:33:00 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/02 19:33:13 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

const char	*ServerConf::getIp() const {
	return m_main.second.listen.first.c_str();
}

size_t	ServerConf::getPort() const {
	return m_main.second.listen.second;
}

const s_base	&ServerConf::getLocationByName( std::string serverName, std::string locationName, bool &yes ) const {
	const s_server &server = getServerByName(serverName);

	std::map<std::string, s_location>::const_iterator it = server.location.find(locationName);
	if (it != server.location.end() && (*it).first == locationName) {
		yes = true;
		return (*it).second;
	}
	yes = false;
	return server;
}

const s_server	&ServerConf::getServerByName( std::string name ) const {
	std::vector<std::string>::const_iterator itMain = std::find(m_main.first.begin(), m_main.first.end(), name);
	if (itMain != m_main.first.end() && (*itMain) == name)
		return m_main.second;
	for (std::map<std::vector<std::string>, s_server>::const_iterator it = m_subs.begin(); it != m_subs.end(); ++it) {
		std::vector<std::string>::const_iterator itSub = std::find((*it).first.begin(), (*it).first.end(), name);
		if (itSub != m_main.first.end() && (*itSub) == name)
			return (*it).second;
	}
	return m_main.second;
}

bool	ServerConf::isMethodAllowed( std::string server_name, std::string location, int method ) const { // TODO never sent empty location
	const char *methods[] = {"GET", "POST", "DELETE"};
	bool	yes;

	std::string	word(methods[method]);
	const s_location &LocationStruct = (const s_location&)getLocationByName(server_name, location, yes);

	if (yes == false)
		return true;
	std::vector<std::string>::const_iterator it;
	it = std::find(LocationStruct.method.begin(), LocationStruct.method.end(), word);
	if (it != LocationStruct.method.end())
		return true;
	return false;
}

size_t	ServerConf::isRedirecting(std::string server_name, std::string location, std::string &url) const { // TODO never sent empty location
	bool	yes;
	const s_base	&serverStruct = getLocationByName(server_name, location, yes);

	if (serverStruct.redirect.first != 0) {
		url = serverStruct.redirect.second;
		return serverStruct.redirect.first;
	}
	url = "";
	return 0;
}

std::string	ServerConf::getLocationPath(std::string server_name, std::string location) const { // TODO never sent empty location
	std::string	path = "";
	const s_server	&serverStruct = getServerByName(server_name);
	path += serverStruct.root;
	std::map<std::string, s_location>::const_iterator it = serverStruct.location.find(location);
	if (it != serverStruct.location.end() && (*it).first == location)
		return path += (*it).first;
	else if (location == "/")
		return path += "/";
	return std::string();
}

std::string	ServerConf::getErrorPage(std::string server_name, std::string location, size_t errorNumber) const { // TODO be carefull if location / exist ??
	bool	yes;
	const s_base	&serverStruct = getLocationByName(server_name, location, yes);

	std::map<size_t, std::string>::const_iterator it = serverStruct.error_page.find(errorNumber);
	if (it == serverStruct.error_page.end())
		return std::string();
	return (*it).second;
}
