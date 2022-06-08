/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetConf.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 17:33:00 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/07 18:13:07 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

const s_server	&ServerConf::getServerByName( const std::string &server_name ) const {
	std::vector<std::string>::const_iterator itMain = std::find(m_main.first.begin(), m_main.first.end(), server_name);

	if (itMain != m_main.first.end() && (*itMain) == server_name)
		return m_main.second;
	for (std::map<std::vector<std::string>, s_server>::const_iterator it = m_subs.begin(); it != m_subs.end(); ++it) {
		std::vector<std::string>::const_iterator itSub = std::find((*it).first.begin(), (*it).first.end(), server_name);

		if (itSub != m_main.first.end() && (*itSub) == server_name)
			return (*it).second;
	}
	return m_main.second;
}

const s_base	&ServerConf::getLocationByName( const std::string &server_name, const std::string &location_name, bool &yes, std::string &rest ) const {
	const s_server	&server = getServerByName(server_name);
	std::string		firstPart = (std::string)location_name;
	std::string 	secondPart;

	while (firstPart.empty() == false) {
		std::map<std::string, s_location>::const_iterator it = server.location.find(firstPart);
		if (it != server.location.end() && (*it).first == firstPart) {
			yes = true;
			rest = secondPart;
			return (*it).second;
		}
		secondPart.insert(0, firstPart.substr(firstPart.rfind('/')));
		firstPart.erase(firstPart.rfind('/'));
	}
	yes = false;
	rest = secondPart;
	return server;
}

std::string	ServerConf::getLocationPath( const std::string &server_name, const std::string &location ) const {
	s_base		baseStruct;
	std::string	path;
	std::string	rest;
	bool		yes = false;

	if (location == "/") {
		baseStruct = getServerByName(server_name);
		return baseStruct.root + location;
	}
	path = getServerByName(server_name).root;
	baseStruct = getLocationByName(server_name, location, yes, rest);
	if (yes == true && baseStruct.root.empty() == false)
		path = baseStruct.root + rest; //TODO "/" between root and rest ?
	else
		path += location;
	return path  + "/";
}

const char	*ServerConf::getIp() const {
	return m_main.second.listen.first.c_str();
}

size_t	ServerConf::getPort() const {
	return m_main.second.listen.second;
}

bool	ServerConf::isMethodAllowed(  const std::string &server_name, const std::string &location, int method ) const {
	const char *methods[] = {"GET", "POST", "DELETE"};
	std::string	rest;
	bool		yes;

	std::string	word(methods[method]);
	const s_location &locationStruct = (const s_location&)getLocationByName(server_name, location, yes, rest);

	if (yes == false)
		return true;
	std::vector<std::string>::const_iterator it;
	it = std::find(locationStruct.method.begin(), locationStruct.method.end(), word);
	if (it != locationStruct.method.end())
		return true;
	return false;
}

std::string ServerConf::getCgiPath( const std::string &server_name, const std::string &location, const std::string &extension) const {
	std::string	rest;
	bool		yes;

	const s_location &locationStruct = (const s_location&)getLocationByName(server_name, location, yes, rest); // TODO how to redirect to the correct php file ?

	if (yes == false)
		return std::string();
	std::map<std::string, std::string>::const_iterator it;

	it = locationStruct.cgi.find(extension);
	if (it != locationStruct.cgi.end())
		return (*it).second;
	return std::string();
}

std::string	ServerConf::getErrorPage( const std::string &server_name, const std::string &location, size_t errorNumber) const {
	s_base		baseStruct;
	std::string	path;
	std::string	rest;
	bool		yes = false;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes, rest);
	path = getServerByName(server_name).root;
	std::map<size_t, std::string>::const_iterator it = baseStruct.error_page.find(errorNumber);

	if (it == baseStruct.error_page.end()) {
		baseStruct = getServerByName(server_name);
		std::map<size_t, std::string>::const_iterator itServer = baseStruct.error_page.find(errorNumber);
		if (itServer == baseStruct.error_page.end())
			return std::string();
		return path + (*itServer).second;
	}
	else if (yes == true && baseStruct.root.empty() == false)
		path = baseStruct.root;
	return path + (*it).second;
}

size_t	ServerConf::isRedirecting( const std::string &server_name, const std::string &location, std::string &url) const {
	s_base		baseStruct;
	std::string	rest;
	bool		yes;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes, rest);
	if (baseStruct.redirect.first != 0) {
		url = baseStruct.redirect.second; // TODO construct path if absolute ?
		return baseStruct.redirect.first; // TODO redirect if rest not empty ?
	}
	url = std::string();
	return 0;
}

bool	ServerConf::isAutoindexOn( const std::string &server_name, const std::string &location ) const {
	s_base		baseStruct;
	std::string	rest;
	bool		yes;

	if (location == "/") {
		baseStruct = getServerByName(server_name);
		return baseStruct.autoindex;
	}
	baseStruct = getLocationByName(server_name, location, yes, rest);
	if (yes == false)
		return false; // TODO server autoindex by default ?
	return baseStruct.autoindex;
}

size_t	ServerConf::getBodySize( const std::string &server_name, const std::string &location ) const {
	s_base		baseStruct;
	std::string	rest;
	bool		yes;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes, rest);
	return baseStruct.client_max_body_size;
}

std::string	ServerConf::getIndex( const std::string &server_name, const std::string &location ) const {
	s_base		baseStruct;
	std::string	rest;
	bool		yes = true;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes, rest);
	if (yes == false)
		return std::string();
	else if ((rest.empty() || rest == "/") && baseStruct.index.empty() == false)
		return getLocationPath(server_name, location) + baseStruct.index;
	return std::string();
}

std::string	ServerConf::isUploadPath( const std::string &server_name, const std::string &location ) const {
	s_base		baseStruct;
	std::string	rest;
	bool		yes = true;

	if (location == "/")
		baseStruct = getServerByName(server_name);
	else
		baseStruct = getLocationByName(server_name, location, yes, rest);
	if (yes == false) // TODO accept rest of location ??
		return std::string();
	else if ((rest.empty() || rest == "/") && baseStruct.upload_pass.empty() == false)
		return getLocationPath(server_name, location) + baseStruct.upload_pass + "/";
	return std::string(); // TODO Is upload pass a specification to add at the end and if doesn't exists return only root + location ??
}
