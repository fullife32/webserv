/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:25:48 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/05 21:54:02 by eassouli         ###   ########.fr       */
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
	if (location.error_page.empty() == false) {
		std::cout << "		Error_page:" << std::endl;
		for (std::map<size_t, std::string>::iterator itErrors = location.error_page.begin(); itErrors != location.error_page.end(); ++itErrors)
			std::cout << "			- " << (*itErrors).first << " > " << (*itErrors).second << std::endl;
	}
	std::cout << "		Client_max_body_size: " << location.client_max_body_size << std::endl;
	if (location.redirect.first != 0)
		std::cout << "		Redirect: " << location.redirect.first << " > " << location.redirect.second << std::endl;
	std::cout << "		Root: " << location.root << std::endl;
	std::cout << "		Autoindex: " << location.autoindex << std::endl;
	std::cout << "		Index:" << location.index << std::endl;
	if (location.upload_pass != "")
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
	if (server.error_page.empty() == false) {
		std::cout << "	Error_page:" << std::endl;
		for (std::map<size_t, std::string>::iterator itErrors = server.error_page.begin(); itErrors != server.error_page.end(); ++itErrors)
			std::cout << "		- " << (*itErrors).first << " > " << (*itErrors).second << std::endl;
	}
	std::cout << "	Client_max_body_size: " << server.client_max_body_size << std::endl;
	if (server.redirect.first != 0)
		std::cout << "	Redirect: " << server.redirect.first << " > " << server.redirect.second << std::endl;
	std::cout << "	Root: " << server.root << std::endl;
	std::cout << "	Autoindex: " << server.autoindex << std::endl;
	std::cout << "	Index:" << server.index << std::endl;
	if (server.upload_pass != "")
		std::cout << "	Upload_pass: " << server.upload_pass << std::endl;
	for (std::map<std::string, struct s_location>::const_iterator it = server.location.begin(); it != server.location.end(); ++it) {
		std::cout << std::endl << "	Location: " << (*it).first << std::endl;
		showConfLocation((*it).second);
	}
}

void ServerConf::showConf(std::vector<ServerConf> &confs) {
	int	i = 0;
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

			std::cout << "Sub: " << j << std::endl;
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

void	ServerConf::getConfTest(std::vector<ServerConf> &confs) {
	std::string url;
	std::cout << "Location \"\" ? " << confs.at(0).getLocationPath("", "") << std::endl;
	std::cout << "Location / ? " << confs.at(0).getLocationPath("simple.com", "/") << std::endl;
	std::cout << "Location other / ? " << confs.at(0).getLocationPath("other_simple.com", "/") << std::endl;
	std::cout << "Location /exec ? " << confs.at(0).getLocationPath("", "/exec") << std::endl;
	std::cout << "Location other /exec ? " << confs.at(0).getLocationPath("other_simple.com", "/exec") << std::endl;
	std::cout << std::endl;

	std::cout << "Get IP ? " << confs.at(0).getIp() << " Get Port ? " << confs.at(0).getPort() << std::endl;
	std::cout << std::endl;

	std::cout << "Method allowed / ? " << confs.at(0).isMethodAllowed("", "/", 0) << std::endl;
	std::cout << "Method allowed GET /test ? " << confs.at(0).isMethodAllowed("", "/test", 0) << std::endl;
	std::cout << "Method allowed GET /get_only ? " << confs.at(0).isMethodAllowed("", "/get_only", 0) << std::endl;
	std::cout << "Method allowed POST /get_only ? " << confs.at(0).isMethodAllowed("", "/get_only", 1) << std::endl;
	std::cout << std::endl;

	std::cout << "Cgi path /exec ? " << confs.at(0).getCgiPath("", "/exec", ".php") << std::endl;
	std::cout << "Cgi path /test ? " << confs.at(0).getCgiPath("", "/test", ".php") << std::endl;
	std::cout << "Cgi path / ? " << confs.at(0).getCgiPath("", "/", ".php") << std::endl;
	std::cout << "Cgi path / 2 ? " << confs.at(0).getCgiPath("", "/", ".py") << std::endl;
	std::cout << "Cgi path other /exec ? " << confs.at(0).getCgiPath("other_simple.com", "/exec", ".py") << std::endl;
	std::cout << std::endl;

	std::cout << "Error page 404 / ? " << confs.at(0).getErrorPage("", "/", 404) << std::endl;
	std::cout << "Error page 404 /test ? " << confs.at(0).getErrorPage("", "/test", 404) << std::endl;
	std::cout << "Error page 404 /get_only ? " << confs.at(0).getErrorPage("", "/get_only", 404) << std::endl;
	std::cout << "Error page 404 /no_get ? " << confs.at(0).getErrorPage("", "/no_get", 404) << std::endl;
	std::cout << "Error page 0 / ? " << confs.at(0).getErrorPage("", "/", 0) << std::endl;
	std::cout << "Error page other / ? " << confs.at(0).getErrorPage("other_simple.com", "/", 403) << std::endl;
	std::cout << std::endl;

	std::cout << "Max body size / ? " << confs.at(0).getBodySize("", "/") << std::endl;
	std::cout << "Max body size /test ? " << confs.at(0).getBodySize("", "/test") << std::endl;
	std::cout << "Max body size /upload ? " << confs.at(0).getBodySize("", "/upload") << std::endl;
	std::cout << "Max body size other /upload ? " << confs.at(0).getBodySize("other_simple.com", "/upload") << std::endl;
	std::cout << std::endl;

	std::cout << "Is redirecting / ? " << confs.at(0).isRedirecting("", "/", url) << " > " << url << std::endl;
	std::cout << "Is redirecting /test ? " << confs.at(0).isRedirecting("", "/test", url) << " > " << url << std::endl;
	std::cout << "Is redirecting /return_page ? " << confs.at(0).isRedirecting("", "/return_page", url) << " > " << url << std::endl;
	std::cout << "Is redirecting other / ? " << confs.at(0).isRedirecting("other_simple.com", "/", url) << " > " << url << std::endl;
	std::cout << "Is redirecting other /return_page ? " << confs.at(0).isRedirecting("other_simple.com", "/return_page", url) << " > " << url << std::endl;
	std::cout << std::endl;

	std::cout << "Is Autoindex / ? " << confs.at(0).isAutoindexOn("", "/") << std::endl;
	std::cout << "Is Autoindex /test ? " << confs.at(0).isAutoindexOn("", "/test") << std::endl;
	std::cout << "Is Autoindex /get_only ? " << confs.at(0).isAutoindexOn("", "/get_only") << std::endl;
	std::cout << "Is Autoindex other / ? " << confs.at(0).isAutoindexOn("other_simple.com", "/") << std::endl;
	std::cout << std::endl;

	std::cout << "Index / ? " << confs.at(0).getIndex("", "/") << std::endl;
	std::cout << "Index /test ? " << confs.at(0).getIndex("", "/test") << std::endl;
	std::cout << "Index /exec ? " << confs.at(0).getIndex("", "/exec") << std::endl;
	std::cout << "Index /upload ? " << confs.at(0).getIndex("", "/upload") << std::endl;
	std::cout << "Index other / ? " << confs.at(0).getIndex("other_simple.com", "/") << std::endl;
	std::cout << std::endl;

	std::cout << "Is Upload path / ? " << confs.at(0).isUploadPath("", "/") << std::endl;
	std::cout << "Is Upload path /test ? " << confs.at(0).isUploadPath("", "/test") << std::endl;
	std::cout << "Is Upload path /upload ? " << confs.at(0).isUploadPath("", "/upload") << std::endl;
	std::cout << "Is Upload path other / ? " << confs.at(0).isUploadPath("other_simple.com", "/") << std::endl;
	std::cout << "Is Upload path other /upload ? " << confs.at(0).isUploadPath("other_simple.com", "/upload") << std::endl;
	std::cout << std::endl;
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
				if (insertInSub(serverTmp, confs) == false) // TODO: accept no server_name if main
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
	// showConf(confs); // TODO DEBUG
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

void	ServerConf::replaceConfig(s_server &server, s_location &location) {
	server.error_page.insert(location.error_page.begin(), location.error_page.end());
	if (location.client_max_body_size != DEFAULT_BODY)
		server.client_max_body_size = location.client_max_body_size;
	if (location.redirect.first != 0)
		server.redirect = location.redirect;
	if (location.root != "")
		server.root = location.root;
	if (server.autoindex == false || location.autoindex != false)
		server.autoindex = location.autoindex;
	if (location.index != "index.html")
		server.index = location.index;
	if (location.upload_pass != "")
		server.upload_pass = location.upload_pass;
}

void	ServerConf::mandatoryCheck( struct s_server &config ) {
	if (config.listen.first == "")
		throw ServerConf::ConfFail(SERVER_MANDATORY, "listen");
	std::map<std::string,s_location>::iterator it = config.location.find("/");
	if (it != config.location.end())
		replaceConfig(config, (*it).second);
	if (config.root == "")
		config.root = "html";
	if (config.index == "")
		config.index = "index.html";
	for (std::map<std::string,s_location>::iterator it = config.location.begin(); it != config.location.end(); ++it) {
		if ((*it).second.root == "")
			(*it).second.root = config.root;
		if ((*it).second.index == "")
			(*it).second.index = "index.html";
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
		char						buf[CONFIG_BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, CONFIG_BUFFER_SIZE, '\n'); // TODO: move in function check arg ??
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
	config.index = "";
	config.upload_pass = "";
}

bool	ServerConf::findServer( std::ifstream &ifs ) {
	while (ifs.good()) {
		char						buf[CONFIG_BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, CONFIG_BUFFER_SIZE, '\n');
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
		char						buf[CONFIG_BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, CONFIG_BUFFER_SIZE, '\n');
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
	config.index = "";
	config.upload_pass = "";
}
