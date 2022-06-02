/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:25:48 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/01 19:26:46 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf( std::vector<std::string> server_name, s_server serverConf )
: m_main(std::make_pair(server_name, serverConf)) { }

ServerConf::ServerConf( ServerConf const &other )
: m_main(other.m_main), m_subs(other.m_subs) { }

ServerConf::~ServerConf() { }

// DEBUG
template <class T>
void	printVector(const std::vector<T> & vec)
{
	typename std::vector<T>::const_iterator	it;

	for (it = vec.begin(); it != vec.end(); it++)
		std::cout << *it << std::endl;
}

static void	showConfLocation(s_location location) {
	std::cout << "		Error_page:" << std::endl;
	for (std::map<size_t, std::string>::iterator itErrors = location.error_page.begin(); itErrors != location.error_page.end(); ++itErrors)
		std::cout << "			- " << (*itErrors).first << " > " << (*itErrors).second << std::endl;
	std::cout << "		Client_max_body_size: " << location.client_max_body_size << std::endl;
	std::cout << "		Redirect: " << location.redirect.first << " > " << location.redirect.second << std::endl;
	std::cout << "		Root: " << location.root << std::endl;
	std::cout << "		Autoindex: " << location.autoindex << std::endl;
	std::cout << "		Index:";
	for (std::vector<std::string>::iterator itIndex = location.index.begin(); itIndex != location.index.end(); ++itIndex)
		std::cout << " " << (*itIndex);
	std::cout << std::endl;
	std::cout << "		Upload_pass: " << location.upload_pass << std::endl;
	std::cout << "		Method:";
	for (std::vector<std::string>::iterator itMethod = location.method.begin(); itMethod != location.method.end(); ++itMethod)
		std::cout << " " << (*itMethod);
	std::cout << std::endl;
	std::cout << "		Cgi:" << std::endl;
	for (std::map<std::string, std::string>::iterator itCgi = location.cgi.begin(); itCgi != location.cgi.end(); ++itCgi)
		std::cout << "			- " << (*itCgi).first << " > " << (*itCgi).second << std::endl;
}

static void	showConfServer(s_server &server) {
	std::cout << "	Error_page:" << std::endl;
	for (std::map<size_t, std::string>::iterator itErrors = server.error_page.begin(); itErrors != server.error_page.end(); ++itErrors)
		std::cout << "		- " << (*itErrors).first << " > " << (*itErrors).second << std::endl;
	std::cout << "	Client_max_body_size: " << server.client_max_body_size << std::endl;
	std::cout << "	Redirect: " << server.redirect.first << " > " << server.redirect.second << std::endl;
	std::cout << "	Root: " << server.root << std::endl;
	std::cout << "	Autoindex: " << server.autoindex << std::endl;
	std::cout << "	Index:";
	for (std::vector<std::string>::iterator itIndex = server.index.begin(); itIndex != server.index.end(); ++itIndex)
		std::cout << " " << (*itIndex);
	std::cout << std::endl;
	std::cout << "	Upload_pass: " << server.upload_pass << std::endl;
	for (std::map<std::string, struct s_location>::const_iterator it = server.location.begin(); it != server.location.end(); ++it) {
		std::cout << std::endl << "	Location: " << (*it).first << std::endl;
		showConfLocation((*it).second);
	}
}

void ServerConf::showConf(std::vector<ServerConf> &confs) {
	int	i = 0;
	std::cout << std::endl << std::endl << std::endl << std::endl;
	for (std::vector<ServerConf>::iterator it = confs.begin(); it != confs.end(); ++it) {
		std::cout << "Server: " << i << std::endl << "Main" << std::endl;
		std::cout << "	Listen: " << (*it).m_main.second.listen.first << " : " << (*it).m_main.second.listen.second << std::endl;
		std::cout << "	Server_name:";
		for (std::vector<std::string>::iterator itNames = (*it).m_main.first.begin(); itNames != (*it).m_main.first.end(); ++itNames)
		std::cout << " " << (*itNames);
		std::cout << std::endl;
		showConfServer((*it).m_main.second);
		std::cout << std::endl;
		int j = 0;
		for (std::map<std::vector<std::string>, s_server>::iterator itSubs = (*it).m_subs.begin(); itSubs != (*it).m_subs.end(); ++itSubs) {

			std::cout << "Sub " << j << ":" << std::endl;
			std::cout << "	Server_name:";
			for (std::vector<std::string>::const_iterator itNamesSubs = (*itSubs).first.begin(); itNamesSubs != (*itSubs).first.end(); ++itNamesSubs)
				std::cout << " " << (*itNamesSubs);
			std::cout << std::endl;
			showConfServer((*itSubs).second);
			std::cout << std::endl;
			j++;
		}
		i++;
	}
}
//

void	ServerConf::setFunctionsCall( parseFunction_t &serverFnct, parseFunction_t &locationFnct ) {
	serverFnct.insert(std::make_pair("listen", parseListen));
	serverFnct.insert(std::make_pair("server_name", parseServerName));
	serverFnct.insert(std::make_pair("error_page", parseErrorPage));
	serverFnct.insert(std::make_pair("client_max_body_size", parseClientMaxBodySize));
	serverFnct.insert(std::make_pair("return", parseRedirect));
	serverFnct.insert(std::make_pair("root", parseRoot));
	serverFnct.insert(std::make_pair("autoindex", parseAutoindex));
	serverFnct.insert(std::make_pair("index", parseIndex));
	serverFnct.insert(std::make_pair("upload_pass", parseUploadPass));

	locationFnct.insert(std::make_pair("method", parseMethod));
	locationFnct.insert(std::make_pair("cgi", parseCgi));
	locationFnct.insert(std::make_pair("error_page", parseErrorPage));
	locationFnct.insert(std::make_pair("client_max_body_size", parseClientMaxBodySize));
	locationFnct.insert(std::make_pair("return", parseRedirect));
	locationFnct.insert(std::make_pair("root", parseRoot));
	locationFnct.insert(std::make_pair("autoindex", parseAutoindex));
	locationFnct.insert(std::make_pair("index", parseIndex));
	locationFnct.insert(std::make_pair("upload_pass", parseUploadPass));
}

void	ServerConf::openFile( std::string filePath, std::ifstream &ifs ) {
	ifs.open(filePath.c_str(), std::ifstream::in);
	if (!ifs.is_open())
		throw ServerConf::ConfFail(NO_FILE, filePath);
	else if (ifs.eof())
		throw ServerConf::ConfFail(EMPTY_FILE, filePath);
	else if (!ifs.good())
		throw ServerConf::ConfFail(NOT_FILE, filePath);
}

int		ServerConf::startParse( const std::string &filePath, std::vector<ServerConf> &confs ) {
	std::ifstream	ifs;

	try {
		ServerConf::openFile(filePath, ifs);
	} catch (std::exception const &except) {
		std::cerr << except.what() << std::endl;
		return 1;
	}

	parseFunction_t	serverFnct;
	parseFunction_t	locationFnct;
	setFunctionsCall(serverFnct, locationFnct);

	try {
		while (ifs.good()) {
			struct s_server serverTmp;

			if (findServer(ifs) == true) {
				initServerConf(serverTmp);
				parseServer(ifs, serverTmp, serverFnct, locationFnct);
				mandatoryCheck(serverTmp);
				if (insertInSub(serverTmp, confs) == false)
					confs.push_back(ServerConf(serverTmp.server_name, serverTmp));
			}
		}
		if (!ifs.eof())
			throw ServerConf::ConfFail(ANOTHER_ERROR, "");
	} catch (ServerConf::ConfFail const &except) {
		if (ifs.is_open())
			ifs.close();
		std::cerr << except.what() << std::endl;
		return 1;
	}

	if (ifs.is_open())
		ifs.close();
	if (confs.empty()) {
		std::cerr << "No server block found in file" << std::endl;
		return 1;
	}
	showConf(confs);
	return 0;
}

bool	ServerConf::insertInSub(s_server &newServer, std::vector<ServerConf> &confs) {
	for (std::vector<ServerConf>::iterator it = confs.begin(); it != confs.end(); ++it) {
		if ((*it).getIp() == newServer.listen.first
		&& (*it).getPort() == newServer.listen.second) {
			(*it).m_subs.insert(make_pair(newServer.server_name, newServer)); // TODO: make private
			return true;
		}
	}
	return false;
}

void	ServerConf::mandatoryCheck( struct s_server &config ) {
	if (config.listen.first == "")
		throw ServerConf::ConfFail(SERVER_MANDATORY, "listen");
	else if (config.server_name.empty() == true)
		throw ServerConf::ConfFail(SERVER_MANDATORY, "server_name");
	std::map<std::string,s_location>::const_iterator it = config.location.find("/");
	if (config.root == "" && (it == config.location.end() || (*it).second.root == ""))
		throw ServerConf::ConfFail(SERVER_MANDATORY, "root");
	if (config.index.empty() == true) {
		config.index.push_back("index");
		config.index.push_back("index.html");
	}
	for (std::map<std::string,s_location>::iterator it = config.location.begin(); it != config.location.end(); ++it) {
		if ((*it).second.index.empty()) {
			(*it).second.index.push_back("index");
			(*it).second.index.push_back("index.html");
		}
		if ((*it).second.method.empty()) {
			(*it).second.method.push_back("GET");
			(*it).second.method.push_back("POST");
			(*it).second.method.push_back("DELETE");
		}
	}
}

bool	ServerConf::isEnding( std::string &lastToken ) {
	return (lastToken.size() != 0 && lastToken[lastToken.length() - 1] == ';');
}

void	ServerConf::parseServer( std::ifstream &ifs, struct s_server &block, parseFunction_t &serverFnct, parseFunction_t &locationFnct ) {
	while (ifs.good()) { // TODO: secure every read loop
		char						buf[BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, BUFFER_SIZE, '\n');
		tokens = splitStringtoTokens(buf, " \t");
		// printVector(tokens); //
		if ((tokens.empty() || tokens[0] == "#") && !ifs.eof())
			continue;
		else if (tokens.empty() && ifs.eof())
			throw ServerConf::ConfFail(NO_CLOSING_BRACKET, "server");
		else if (tokens.size() == 1 && tokens[0] == "}")
			return;
		else if (tokens[0] == "location") {
			if (tokens.size() != 3 || tokens[2] != "{")
				throw ServerConf::ConfFail(NOT_VALID_KEY, "location");
			else if (tokens[1][0] != '/')
				throw ServerConf::ConfFail(WRONG_PATH_FORMAT, tokens[1]);
			struct s_location locationTmp;

			initLocationConf(locationTmp);
			parseLocation(ifs, locationTmp, locationFnct);
			block.location.insert(std::make_pair(tokens[1], locationTmp));
		}
		else {
			parseFunction_t::iterator	it;

			if (isEnding(tokens.back()) == false)
				throw ServerConf::ConfFail(MISSING_END_LINE, tokens[0]);
			popLast(tokens.back());
			if (tokens.back().length() == 0)
				tokens.pop_back();
			it = serverFnct.find(*tokens.begin());
			if (it == serverFnct.end())
				throw ServerConf::ConfFail(NOT_VALID_KEY, tokens[0]);
			if (tokens.size() < 2)
				throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0]);
			it->second(tokens, block);
		}
	}
	if (ifs.eof())
		throw ServerConf::ConfFail(NO_CLOSING_BRACKET, "server");
}

void	ServerConf::initServerConf( struct s_server &config ) {
	config.listen = std::make_pair("", 0);
	config.client_max_body_size = DEFAULT_BODY;
	config.root = "";
	config.autoindex = false;
	config.upload_pass = "";
}

bool	ServerConf::findServer( std::ifstream &ifs ) {
	while (ifs.good()) {
		char						buf[BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, BUFFER_SIZE, '\n');
		tokens = splitStringtoTokens(buf, " \t");
		if (tokens.empty() || tokens[0] == "#")
			continue;
		else if (tokens.size() == 2 && tokens[0] == "server" && tokens[1] == "{")
			return true;
		else
			throw ServerConf::ConfFail(OUTSIDE_SERVER, tokens[0]);
	}
	return false;
}

void ServerConf::parseLocation( std::ifstream &ifs, struct s_location &location, parseFunction_t &locationFnct ) {
	while (ifs.good()) {
		char						buf[BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, BUFFER_SIZE, '\n');
		tokens = splitStringtoTokens(buf, " \t");
		// printVector(tokens); //
		if ((tokens.empty() || tokens[0] == "#") && !ifs.eof())
			continue;
		else if (tokens.empty() && ifs.eof())
			throw ServerConf::ConfFail(NO_CLOSING_BRACKET, "location");
		else if (tokens.size() == 1 && tokens[0] == "}")
			return;
		else {
			parseFunction_t::iterator	it;

			if (isEnding(tokens.back()) == false)
				throw ServerConf::ConfFail(MISSING_END_LINE, tokens[0]);
			popLast(tokens.back());
			if (tokens.back().length() == 0)
				tokens.pop_back();
			it = locationFnct.find(*tokens.begin());
			if (it == locationFnct.end())
				throw ServerConf::ConfFail(NOT_VALID_KEY, tokens[0]);
			if (tokens.size() < 2)
				throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0]);
			it->second(tokens, location);
		}
	}
}

void	ServerConf::initLocationConf( struct s_location &config ) {
	config.client_max_body_size = DEFAULT_BODY;
	config.root = "";
	config.autoindex = false;
	config.upload_pass = "";
}
