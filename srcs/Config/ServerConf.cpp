/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:25:48 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/30 22:53:10 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf( s_server serverConf )
: m_main(serverConf) { }

ServerConf::ServerConf( ServerConf const &other )
: m_main(other.m_main), m_subs(other.m_subs) { }

ServerConf::~ServerConf() { }

template <class T>
void	printVector(const std::vector<T> & vec)
{
	typename std::vector<T>::const_iterator	it;

	for (it = vec.begin(); it != vec.end(); it++)
		std::cout << *it << std::endl;
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
			struct s_server confTmp;

			if (findServer(ifs) == true) {
				// init structs here
				parseServer(ifs, confTmp, serverFnct, locationFnct);
				// if () server ip and port equal another server set to his sub
				confs.push_back(ServerConf(confTmp));
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

void	ServerConf::openFile( std::string filePath, std::ifstream &ifs ) { // doesn't work with directory
	ifs.open(filePath.c_str(), std::ifstream::in);
	if (!ifs.is_open())
		throw ServerConf::ConfFail(NO_FILE, filePath.c_str());
	else if (ifs.eof())
		throw ServerConf::ConfFail(EMPTY_FILE, filePath.c_str()); // Usefull ?
	else if (!ifs.good())
		throw ServerConf::ConfFail(NOT_FILE, filePath.c_str());
}

void	ServerConf::setFunctionsCall( parseFunction_t &serverFnct, parseFunction_t &locationFnct ) {
	serverFnct.insert(std::make_pair("listen", parseListen));
	serverFnct.insert(std::make_pair("server_name", parseServerName));
	serverFnct.insert(std::make_pair("error_page", parseErrorPage));
	serverFnct.insert(std::make_pair("client_max_body_size", parseClientMaxBodySize));
	serverFnct.insert(std::make_pair("return", parseRedirect));
	// serverFnct.insert(std::make_pair("root", parseRoot));
	// serverFnct.insert(std::make_pair("autoindex", parseAutoindex));
	// serverFnct.insert(std::make_pair("index", parseIndex));
	// serverFnct.insert(std::make_pair("upload_pass", parseUploadPass));

	// locationFnct.insert(std::make_pair("method", parseMethod));
	// locationFnct.insert(std::make_pair("cgi", parseCgi));
	locationFnct.insert(std::make_pair("error_page", parseErrorPage));
	locationFnct.insert(std::make_pair("client_max_body_size", parseClientMaxBodySize));
	locationFnct.insert(std::make_pair("return", parseRedirect));
	// locationFnct.insert(std::make_pair("root", parseRoot));
	// locationFnct.insert(std::make_pair("autoindex", parseAutoindex));
	// locationFnct.insert(std::make_pair("index", parseIndex));
	// locationFnct.insert(std::make_pair("upload_pass", parseUploadPass));
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
			throw ServerConf::ConfFail(OUTSIDE_SERVER, tokens[0].c_str());
	}
	return false;
}

static bool	isEnding( std::string &lastToken ) {
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
		else if (tokens[0] == "location")
			; // function location with location map
		else {
			parseFunction_t::iterator	it;

			if (isEnding(tokens.back()) == false)
				throw ServerConf::ConfFail(MISSING_END_LINE, tokens[0].c_str()); // return all line instead ? with spaces ?
			popLast(tokens.back());
			if (tokens.back().length() == 0)
				tokens.pop_back();
			it = serverFnct.find(*tokens.begin());
			if (it == serverFnct.end())
				throw ServerConf::ConfFail(NOT_VALID_KEY, tokens[0].c_str()); // Not a valid key send token and nb of error
				// throw ServerConf::ConfFail(NOT_VALID_KEY, tokens[0].c_str()); // Not a valid key send token and nb of error
			if (tokens.size() < 2)
				throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0].c_str());
			it->second(tokens, block);
		}
	}
	if (block.listen.second == 0)
		throw ServerConf::ConfFail(SERVER_MANDATORY, "no host/port"); // Not a valid key send token and nb of error
	// check si config suffisante
}

static void	isValidIpAddress( std::string &ip ) {
	if (ip == "localhost")
		return;
	else if (ip == IP_MASK)
		throw ServerConf::ConfFail(INVALID_IP, ip.c_str());
	std::vector<std::string>	addr;

	addr = splitString(ip, ".");
	if (addr.size() != 4 || ip[ip.size() - 1] < '0' || ip[ip.size() - 1] > '9')
		throw ServerConf::ConfFail(INVALID_IP, ip.c_str());
	for (std::vector<std::string>::iterator it = addr.begin(); it != addr.end(); ++it) {
		if ((*it).length() == 0 || ((*it).length() != 1 && (*it)[0] == '0'))
			throw ServerConf::ConfFail(INVALID_IP, ip.c_str());
		char	*pEnd;
		long	n;

		n = strtol((*it).c_str(), &pEnd, 10);
		if (n < 0 || n > 255)
			throw ServerConf::ConfFail(INVALID_IP, ip.c_str());
		else if (std::strlen(pEnd) != 0)
			throw ServerConf::ConfFail(INVALID_PORT, ip.c_str());
	}
	// TODO: Fix .0.0.0:xxx
}

static void	isValidPort( std::string &port ) {
	char	*pEnd;
	long	n;

	if (port[0] == '0')
		throw ServerConf::ConfFail(INVALID_PORT, port.c_str());
	n = strtol(port.c_str(), &pEnd, 10);
	if (n <= 0 || n > MAX_PORT)
		throw ServerConf::ConfFail(INVALID_PORT, port.c_str());
	else if (std::strlen(pEnd) != 0)
		throw ServerConf::ConfFail(INVALID_PORT, port.c_str());
}

static bool	isPort( std::string &port ) {
	char	*pEnd;

	strtol(port.c_str(), &pEnd, 10);
	return (std::strlen(pEnd) == 0);
}

void	ServerConf::parseListen( std::vector<std::string> &tokens, struct s_server &block ) {
	if (block.listen.first != "")
		throw ServerConf::ConfFail(TOO_MANY_DIRECTIVE, tokens[0].c_str());
	if (tokens.size() != 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0].c_str());
	std::vector<std::string>	inet;

	inet.push_back(LOCALHOST); // TODO: or 0.0.0.0 ??
	inet.push_back(DEFAULT_PORT);
	if (tokens[1].find(':') != std::string::npos) {
		inet[0] = tokens[1].substr(0, tokens[1].find(':'));
		inet[1] = tokens[1].substr(tokens[1].find(':') + 1);
	}
	else if (isPort(tokens[1]) == true)
		inet[1] = tokens[1];
	else {
		if (tokens[1] != "localhost")
			inet[0] = tokens[1];
	}
	isValidIpAddress(inet[0]);
	isValidPort(inet[1]);
	block.listen = std::make_pair(inet[0], strtod(inet[1].c_str(), NULL));
}

void	ServerConf::parseServerName( std::vector<std::string> &tokens, struct s_server &block ) {
	if (!block.server_name.empty()) // many server name entrance possible ?
		throw ServerConf::ConfFail(TOO_MANY_DIRECTIVE, tokens[0].c_str());
	for (std::vector<std::string>::iterator it = tokens.begin() + 1; it != tokens.end(); ++it) {
		block.server_name.push_back(*it); // check format ?
	}
}

void	ServerConf::parseErrorPage( std::vector<std::string> &tokens, struct s_server &block ) { // TODO: check format URI ?
	if (tokens.size() < 3)
		throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0].c_str());
	// check format uri last element
	for (std::vector<std::string>::iterator code = tokens.begin() + 1; *code != tokens.back(); ++code) {
		double	n;

		if ((*code).size() != 3)
			throw ServerConf::ConfFail(WRONG_ERROR_HTTP_CODE, (*code).c_str());
		n = std::strtod((*code).c_str(), NULL); // what to do if code already set to uri ?
		if (n < 400 || n > 599)
			throw ServerConf::ConfFail(WRONG_ERROR_HTTP_CODE, (*code).c_str()); // code out of range ?
		block.error_page.insert(std::make_pair(n, tokens.back()));
	}
}

void	ServerConf::parseClientMaxBodySize( std::vector<std::string> &tokens, struct s_server &block ) {
	if (tokens.size() > 2)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0].c_str()); // code out of range ?
	double	n;
	char	*pEnd = NULL;

	n = std::strtod(tokens[1].c_str(), &pEnd); // what to do if already entered
	if (n != 0 && tokens[1].at(0) == '0')
		throw ServerConf::ConfFail(WRONG_BODY_SIZE_FORMAT, tokens[1].c_str());
	else if (pEnd != NULL && std::strlen(pEnd) > 1)
		throw ServerConf::ConfFail(WRONG_BODY_SIZE_FORMAT, tokens[1].c_str());
	else if (pEnd != NULL && std::strlen(pEnd) == 1 && std::tolower(*pEnd) != 'm')
		throw ServerConf::ConfFail(WRONG_BODY_SIZE_FORMAT, tokens[1].c_str());
	if (pEnd != NULL && std::tolower(*pEnd) == 'm')
		n *= 1000000;
	block.client_max_body_size = n;
}

void	ServerConf::parseRedirect( std::vector<std::string> &tokens, struct s_server &block ) {
	if (tokens.size() < 3)
		throw ServerConf::ConfFail(NOT_ENOUGH_ARGUMENTS, tokens[0].c_str());
	if (tokens.size() > 3)
		throw ServerConf::ConfFail(TOO_MANY_ARGUMENTS, tokens[0].c_str());
	// check format uri last element
	double	n;

	if (tokens[1].size() != 3)
		throw ServerConf::ConfFail(WRONG_REDIR_HTTP_CODE, tokens[1].c_str());
	n = std::strtod(tokens[1].c_str(), NULL); // what to do if code already set to uri ?
	if (n < 300 || n > 399)
		throw ServerConf::ConfFail(WRONG_REDIR_HTTP_CODE, tokens[1].c_str()); // code out of range ?
	block.error_page.insert(std::make_pair(n, tokens[2]));
}
