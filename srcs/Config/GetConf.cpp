/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetConf.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 17:33:00 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/03 19:35:29 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

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

const s_server	&ServerConf::getServerByName( std::string serverName ) const {
	std::vector<std::string>::const_iterator itMain = std::find(m_main.first.begin(), m_main.first.end(), serverName);

	if (itMain != m_main.first.end() && (*itMain) == serverName)
		return m_main.second;
	for (std::map<std::vector<std::string>, s_server>::const_iterator it = m_subs.begin(); it != m_subs.end(); ++it) {
		std::vector<std::string>::const_iterator itSub = std::find((*it).first.begin(), (*it).first.end(), serverName);

		if (itSub != m_main.first.end() && (*itSub) == serverName)
			return (*it).second;
	}
	return m_main.second;
}

std::string	ServerConf::getLocationPath(std::string server_name, std::string location) const { // TODO to know if location exists
	s_base		baseStruct;
	std::string	path = "";
	bool		yes;

	if (location == "/") {
		baseStruct = getServerByName(server_name);
		return baseStruct.root;
	}
	baseStruct = getLocationByName(server_name, location, yes);
	if (yes == false)
		return std::string();
	else
		return baseStruct.root + location;
}

const char	*ServerConf::getIp() const {
	return m_main.second.listen.first.c_str();
}

size_t	ServerConf::getPort() const {
	return m_main.second.listen.second;
}

bool	ServerConf::isMethodAllowed( std::string server_name, std::string location, int method ) const { // TODO never sent empty location
	const char *methods[] = {"GET", "POST", "DELETE"};
	bool		yes;

	std::string	word(methods[method]);
	const s_location &locationStruct = (const s_location&)getLocationByName(server_name, location, yes);

	if (yes == false)
		return true;
	std::vector<std::string>::const_iterator it;
	it = std::find(locationStruct.method.begin(), locationStruct.method.end(), word);
	if (it != locationStruct.method.end())
		return true;
	return false;
}

std::string ServerConf::getCgiPath(std::string server_name, std::string location, std::string extension) const {
	std::string	path = "";
	bool	yes;

	const s_location &locationStruct = (const s_location&)getLocationByName(server_name, location, yes);

	if (yes == false)
		return std::string();
	std::map<std::string, std::string>::const_iterator it;

	it = locationStruct.cgi.find(extension);
	if (it != locationStruct.cgi.end())
		return (*it).second;
	return std::string();
}

std::string	ServerConf::getErrorPage(std::string server_name, std::string location, size_t errorNumber) const { // TODO be carefull if location / exist ??
	s_base	baseStruct;
	std::string	path = "";
	bool	yes;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes);
	std::map<size_t, std::string>::const_iterator it = baseStruct.error_page.find(errorNumber);

	if (it == baseStruct.error_page.end() && yes == true) {
		baseStruct = getServerByName(server_name);
		std::map<size_t, std::string>::const_iterator itServer = baseStruct.error_page.find(errorNumber);
		if (itServer == baseStruct.error_page.end())
			return std::string();
		return baseStruct.root + (*itServer).second;
	}
	if (yes == false)
		path = baseStruct.root;
	else
		path = getLocationPath(server_name, location);
	return path + (*it).second;
}

size_t	ServerConf::isRedirecting(std::string server_name, std::string location, std::string &url) const { // TODO never sent empty location
	s_base	baseStruct;
	bool	yes;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes);
	if (baseStruct.redirect.first != 0) {
		url = baseStruct.redirect.second;
		return baseStruct.redirect.first;
	}
	url = "";
	return 0;
}

bool	ServerConf::isAutoindexOn(std::string server_name, std::string location) const {
	s_base	baseStruct;
	bool	yes;

	baseStruct = getLocationByName(server_name, location, yes);
	return baseStruct.autoindex;
}

size_t	ServerConf::getBodySize(std::string server_name, std::string location) const {
	s_base	baseStruct;
	bool	yes;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes);
	return baseStruct.client_max_body_size;
}

std::vector<std::string>	ServerConf::getIndexList(std::string server_name, std::string location) const {
	s_base	baseStruct;
	bool	yes;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes);
	return baseStruct.index;
}

std::string	ServerConf::isUploadPath(std::string server_name, std::string location) const {
	s_base		baseStruct;
	bool		yes = true;
	std::string	path = "";

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes);
	if (yes == false || baseStruct.upload_pass == "")
		return std::string();
	path = getLocationPath(server_name, location) + baseStruct.upload_pass;
	return path;
}
