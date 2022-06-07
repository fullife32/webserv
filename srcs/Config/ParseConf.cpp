/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConf.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 18:18:34 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/07 15:51:52 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

static void	isValidIpAddress( std::string &ip ) {
	if (ip == "localhost")
		return;
	else if (ip == IP_MASK)
		throw ServerConf::ConfFail(INVALID_IP, ip);
	std::vector<std::string>	addr;

	addr = splitString(ip, ".");
	if (addr.size() != 4 || ip[ip.size() - 1] < '0' || ip[ip.size() - 1] > '9')
		throw ServerConf::ConfFail(INVALID_IP, ip);
	for (std::vector<std::string>::iterator it = addr.begin(); it != addr.end(); ++it) {
		if ((*it).length() == 0 || ((*it).length() != 1 && (*it)[0] == '0'))
			throw ServerConf::ConfFail(INVALID_IP, ip);
		char	*pEnd = NULL;
		long	n;

		n = strtol((*it).c_str(), &pEnd, 10);
		if (n < 0 || n > 255)
			throw ServerConf::ConfFail(INVALID_IP, ip);
		else if (std::strlen(pEnd) != 0)
			throw ServerConf::ConfFail(INVALID_PORT, ip);
	}
}

static void	isValidPort( std::string &port ) {
	char	*pEnd = NULL;
	long	n;

	if (port[0] == '0')
		throw ServerConf::ConfFail(INVALID_PORT, port);
	n = strtol(port.c_str(), &pEnd, 10);
	if (n <= 0 || n > MAX_PORT)
		throw ServerConf::ConfFail(INVALID_PORT, port);
	else if (std::strlen(pEnd) != 0)
		throw ServerConf::ConfFail(INVALID_PORT, port);
}

static bool	isPort( std::string &port ) {
	char	*pEnd = NULL;

	strtol(port.c_str(), &pEnd, 10);
	return (std::strlen(pEnd) == 0);
}

void	ServerConf::parseListen( std::vector<std::string> &tokens, struct s_base &block ) {
	s_server	&blockServer = (s_server&)block;

	if (blockServer.listen.first.empty() == false)
		throw ServerConf::ConfFail(TOO_MANY_DIRECTIVE, tokens[0]);
	if (tokens.size() != 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	std::pair<std::string, std::string>	inet;

	inet.first = LOCALHOST;
	inet.second = DEFAULT_PORT;
	if (tokens[1].find(':') != std::string::npos) {
		inet.first = tokens[1].substr(0, tokens[1].find(':'));
		inet.second = tokens[1].substr(tokens[1].find(':') + 1);
	}
	else if (isPort(tokens[1]) == true)
		inet.second = tokens[1];
	else
		inet.first = tokens[1];
	isValidIpAddress(inet.first);
	isValidPort(inet.second);
	if (inet.first == "localhost")
		inet.first = LOCALHOST;
	blockServer.listen = std::make_pair(inet.first, strtod(inet.second.c_str(), NULL));
}

void	ServerConf::parseServerName( std::vector<std::string> &tokens, struct s_base &block ) {
	s_server	&blockServer = (s_server&)block;

	if (!blockServer.server_name.empty())
		throw ServerConf::ConfFail(TOO_MANY_DIRECTIVE, tokens[0]);
	for (std::vector<std::string>::iterator it = tokens.begin() + 1; it != tokens.end(); ++it) {
		blockServer.server_name.push_back(*it);
	}
}

void	ServerConf::parseMethod( std::vector<std::string> &tokens, struct s_base &block ) {
	s_location	&blockLoc = (s_location&)block;

	for (std::vector<std::string>::iterator methods = tokens.begin() + 1; methods != tokens.end(); ++methods) {
		if (*methods != "GET" && *methods != "POST" && *methods != "DELETE")
			throw ServerConf::ConfFail(WRONG_METHOD, (*methods).c_str());
		blockLoc.method.push_back(*methods);
	}
}

void	ServerConf::parseCgi( std::vector<std::string> &tokens, struct s_base &block ) {
	s_location	&blockLoc = (s_location&)block;

	if (tokens.size() < 3)
		throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0]);
	else if (tokens.size() > 3)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	if (tokens[1][0] != '.' || (tokens[1][0] == '.' && tokens[1].size() == 1))
		throw ServerConf::ConfFail(WRONG_EXT_FORMAT, tokens[1]); // TODO accept only absolute path ?
	blockLoc.cgi.insert(make_pair(tokens[1], tokens[2]));
}

void	ServerConf::parseErrorPage( std::vector<std::string> &tokens, struct s_base &block ) {
	if (tokens.size() < 3)
		throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0]);
	else if (tokens.back()[0] != '/')
		throw ServerConf::ConfFail(WRONG_PATH_FORMAT, tokens.back());
	for (std::vector<std::string>::iterator code = tokens.begin() + 1; *code != tokens.back(); ++code) {
		double	n;

		if ((*code).size() != 3)
			throw ServerConf::ConfFail(WRONG_ERROR_HTTP_CODE, (*code));
		n = std::strtod((*code).c_str(), NULL);
		if (n < 400 || n > 599)
			throw ServerConf::ConfFail(WRONG_ERROR_HTTP_CODE, (*code));
		block.error_page.insert(std::make_pair(n, tokens.back()));
	}
}

void	ServerConf::parseClientMaxBodySize( std::vector<std::string> &tokens, struct s_base &block ) {
	if (tokens.size() > 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	double	n;
	char	*pEnd = NULL;

	n = std::strtod(tokens[1].c_str(), &pEnd);
	if (n != 0 && tokens[1].at(0) == '0')
		throw ServerConf::ConfFail(WRONG_BODY_SIZE_FORMAT, tokens[1]);
	else if (pEnd != NULL && std::strlen(pEnd) > 1)
		throw ServerConf::ConfFail(WRONG_BODY_SIZE_FORMAT, tokens[1]);
	else if (pEnd != NULL && std::strlen(pEnd) == 1 && std::tolower(*pEnd) != 'm')
		throw ServerConf::ConfFail(WRONG_BODY_SIZE_FORMAT, tokens[1]);
	if (pEnd != NULL && std::tolower(*pEnd) == 'm')
		n *= 1000000;
	block.client_max_body_size = n;
}

void	ServerConf::parseRedirect( std::vector<std::string> &tokens, struct s_base &block ) { // TODO: redirect if go to folder or server that redirect ?
	if (tokens.size() < 3)
		throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0]);
	if (tokens.size() > 3)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	double	n;

	if (tokens[1].size() != 3)
		throw ServerConf::ConfFail(WRONG_REDIR_HTTP_CODE, tokens[1]);
	n = std::strtod(tokens[1].c_str(), NULL);
	if (n < 300 || n > 399)
		throw ServerConf::ConfFail(WRONG_REDIR_HTTP_CODE, tokens[1]);
	block.redirect = std::make_pair(n, tokens[2]);
}

void	ServerConf::parseRoot( std::vector<std::string> &tokens, struct s_base &block ) {
	if (tokens.size() != 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	else if (block.root.empty() == false)
		throw ServerConf::ConfFail(TOO_MANY_DIRECTIVE, tokens[0]);
	block.root = tokens[1];
}

void	ServerConf::parseAutoindex( std::vector<std::string> &tokens, struct s_base &block ) {
	if (tokens.size() != 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	else if (tokens[1] != "on" && tokens[1] != "off")
		throw ServerConf::ConfFail(AUTOINDEX_FORMAT, tokens[1]);
	if (tokens[1] == "on")
		block.autoindex = true;
	else if (tokens[1] == "off")
		block.autoindex = false;
}

void	ServerConf::parseIndex( std::vector<std::string> &tokens, struct s_base &block ) {
	if (tokens.size() != 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	if (std::find(tokens[1].begin(), tokens[1].end(), '.') == tokens[1].end())
		throw ServerConf::ConfFail(WRONG_INDEX_FORMAT, tokens[1]);
	block.index = tokens[1];
}

void	ServerConf::parseUploadPass( std::vector<std::string> &tokens, struct s_base &block ) {
	if (tokens.size() > 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0]);
	else if (tokens[1][0] != '/')
		throw ServerConf::ConfFail(WRONG_PATH_FORMAT, tokens[1]);
	block.upload_pass = tokens[1];
}
