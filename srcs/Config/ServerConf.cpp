/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:25:48 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/31 18:36:51 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf( s_server serverConf )
: m_main(serverConf) { }

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

void	ServerConf::openFile( std::string filePath, std::ifstream &ifs ) { // doesn't work with directory
	ifs.open(filePath.c_str(), std::ifstream::in);
	if (!ifs.is_open())
		throw ServerConf::ConfFail(NO_FILE, filePath);
	else if (ifs.eof())
		throw ServerConf::ConfFail(EMPTY_FILE, filePath); // Usefull ?
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
				// if () server ip and port equal another server set to his sub
				confs.push_back(ServerConf(serverTmp));
			}
		}
		if (!ifs.eof())
			throw ServerConf::ConfFail(ANOTHER_ERROR, "read error"); // something failed
		// TODO: if (eof && no server ) print no server block here
	} catch (ServerConf::ConfFail const &except) {
		if (ifs.is_open())
			ifs.close();
		std::cerr << except.what() << std::endl;
		return 1;
	}

	if (ifs.is_open())
		ifs.close();
	if (confs.empty()) {
		std::cerr << "Error: No server block found in file" << std::endl; // two errors printed (catch and this one :/)
		return 1;
	}
	std::cout << "CONFIG FINISHED !" << std::endl; //
	return 0;
}

void	ServerConf::mandatoryCheck( const struct s_server &config ) {
	if (config.listen.first == "")
		throw ServerConf::ConfFail(SERVER_MANDATORY, "listen");
	else if (config.server_name.empty() == true)
		throw ServerConf::ConfFail(SERVER_MANDATORY, "server_name");
	if (config.root == "") // add check root in location /
		throw ServerConf::ConfFail(SERVER_MANDATORY, "root");
	// move here default param if nothing
}

bool	ServerConf::isEnding( std::string &lastToken ) {
	return (lastToken.size() != 0 && lastToken[lastToken.length() - 1] == ';');
}

void	ServerConf::parseServer( std::ifstream &ifs, struct s_server &block, parseFunction_t &serverFnct, parseFunction_t &locationFnct ) {
	while (ifs.good()) { // secure every read loop
		char						buf[BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, BUFFER_SIZE, '\n');
		tokens = splitStringtoTokens(buf, " \t");
		printVector(tokens); //
		if (tokens.empty() && !ifs.eof())
			continue;
		else if (tokens.empty() && ifs.eof())
			throw ServerConf::ConfFail(NO_CLOSING_BRACKET, "server");
		else if (tokens.size() == 1 && tokens[0] == "}")
			return;
		else if (tokens[0] == "location") {
			if (tokens.size() != 3 || tokens[2] != "{")
				throw ServerConf::ConfFail(NOT_VALID_KEY, "location");
			struct s_location locationTmp;

			initLocationConf(locationTmp);
			parseLocation(ifs, locationTmp, locationFnct);
			block.location.insert(std::make_pair(tokens[1], locationTmp));
		}
		else {
			parseFunction_t::iterator	it;

			if (isEnding(tokens.back()) == false)
				throw ServerConf::ConfFail(MISSING_END_LINE, tokens[0]); // return all line instead ? with spaces ?
			popLast(tokens.back());
			if (tokens.back().length() == 0)
				tokens.pop_back();
			it = serverFnct.find(*tokens.begin());
			if (it == serverFnct.end())
				throw ServerConf::ConfFail(NOT_VALID_KEY, tokens[0]); // Not a valid key send token and nb of error
				// throw ServerConf::ConfFail(NOT_VALID_KEY, tokens[0]); // Not a valid key send token and nb of error
			if (tokens.size() < 2)
				throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0]);
			it->second(tokens, block);
		}
	}
	if (ifs.eof())
		throw ServerConf::ConfFail(NO_CLOSING_BRACKET, "server");
}

void	ServerConf::initServerConf( struct s_server &config ) {
	config.listen = std::make_pair("", 0); // mandatory
	// server name mandatory ?
	config.client_max_body_size = DEFAULT_BODY;
	config.root = ""; // mandatory
	config.autoindex = false;
	config.upload_pass = "";
}

bool	ServerConf::findServer( std::ifstream &ifs ) {
	while (ifs.good()) {
		char						buf[BUFFER_SIZE];
		std::vector<std::string>	tokens;

		ifs.getline(buf, BUFFER_SIZE, '\n');
		tokens = splitStringtoTokens(buf, " \t");
		if (tokens.empty())
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
		printVector(tokens); //
		if (tokens.empty() && !ifs.eof())
			continue;
		else if (tokens.empty() && ifs.eof())
			throw ServerConf::ConfFail(NO_CLOSING_BRACKET, "location");
		else if (tokens.size() == 1 && tokens[0] == "}")
			return;
		else {
			parseFunction_t::iterator	it;

			if (isEnding(tokens.back()) == false)
				throw ServerConf::ConfFail(MISSING_END_LINE, tokens[0]); // return all line instead ? with spaces ?
			popLast(tokens.back());
			if (tokens.back().length() == 0)
				tokens.pop_back();
			it = locationFnct.find(*tokens.begin());
			if (it == locationFnct.end())
				throw ServerConf::ConfFail(NOT_VALID_KEY, tokens[0]); // Not a valid key send token and nb of error
			if (tokens.size() < 2)
				throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0]);
			it->second(tokens, location);
		}
	}
}

void	ServerConf::initLocationConf( struct s_location &config ) { // TODO: a revoir
	config.client_max_body_size = DEFAULT_BODY;
	config.method.push_back("GET"); // move after check succeed
	config.method.push_back("POST");
	config.method.push_back("DELETE");
	config.root = "";
	config.autoindex = false;
	config.upload_pass = "";
}
