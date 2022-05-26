/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 16:25:48 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/26 21:04:16 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf( s_server serverConf )
: m_main(serverConf) { }

ServerConf::ServerConf( ServerConf const &other )
: m_main(other.m_main), m_subs(other.m_subs) { }

ServerConf::~ServerConf() { }

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

			memset(&confTmp, 0, sizeof(confTmp));
			parseServer(ifs, confTmp, serverFnct, locationFnct);
			// if () server ip and port equal another server set to his sub
			confs.push_back(ServerConf(confTmp));
		}
		if (!ifs.eof())
			throw ServerConf::ConfFail(); // something failed
	} catch (std::exception &except) {
		if (ifs.is_open())
			ifs.close();
		std::cerr << except.what() << std::endl;
		return 1;
	}

	if (ifs.is_open())
		ifs.close();
	if (confs.empty()) {
		std::cerr << "Error: No server block found in file" << std::endl;
		return 1;
	}
	std::cout << "CONFIG FINISHED !" << std::endl; //
	return 0;
}

void	ServerConf::openFile( std::string filePath, std::ifstream &ifs ) {
	ifs.open(filePath.c_str(), std::ifstream::in);
	if (ifs.eof()) {
		throw ServerConf::ConfFail(); // Empty file
	}
	else if (!ifs.good()) {
		throw ServerConf::ConfFail(); // Impossible to open the file
	}
}

void	ServerConf::setFunctionsCall( parseFunction_t &serverFnct, parseFunction_t &locationFnct ) {
	serverFnct.insert(make_pair("listen", parseListen));
	// serverFnct.insert(make_pair("server_name", parseServerName));
	// serverFnct.insert(make_pair("error_page", parseErrorPage));
	// serverFnct.insert(make_pair("client_max_body_size", parseClientMaxBodySize));
	// serverFnct.insert(make_pair("redirect", parseRedirect));
	// serverFnct.insert(make_pair("root", parseRoot));
	// serverFnct.insert(make_pair("autoindex", parseAutoindex));
	// serverFnct.insert(make_pair("index", parseIndex));
	// serverFnct.insert(make_pair("upload_pass", parseUploadPass));

	(void)locationFnct;
	// locationFnct.insert(make_pair("method", parseMethod));
	// locationFnct.insert(make_pair("cgi", parseCgi));
	// locationFnct.insert(make_pair("error_page", parseErrorPage));
	// locationFnct.insert(make_pair("client_max_body_size", parseClientMaxBodySize));
	// locationFnct.insert(make_pair("redirect", parseRedirect));
	// locationFnct.insert(make_pair("root", parseRoot));
	// locationFnct.insert(make_pair("autoindex", parseAutoindex));
	// locationFnct.insert(make_pair("index", parseIndex));
	// locationFnct.insert(make_pair("upload_pass", parseUploadPass));
}

void	ServerConf::parseServer( std::ifstream &ifs, struct s_server &block, parseFunction_t &serverFnct, parseFunction_t &locationFnct ) {
	int		line = 0;

	findServer(ifs);
	while (ifs.good()) {
		char						buf[256];
		std::vector<std::string>	tokens;

		ifs.getline(buf, 256, '\n');
		tokens = splitString(std::string(buf), " ");
		std::cout << "stop: " << buf << std::endl;
		if (tokens.empty())
			continue;
		else if (tokens.size() == 1 && tokens[0] == "}")
			break;
		else if (tokens[0] == "location")
			; // function location with location map
		else {
			parseFunction_t::iterator	it;
			it = serverFnct.find(*tokens.begin());
			if (it == serverFnct.end())
				throw ServerConf::ConfFail(); // Not a valid key send token and nb of error
			it->second(tokens);

		}
	}
	if (block.listen.second == 0)
		throw ServerConf::ConfFail(); // Not a valid key send token and nb of error
	// check si config suffisante
}

void	ServerConf::findServer( std::ifstream &ifs ) {
	while (ifs.good()) {
		char						buf[256];
		std::vector<std::string>	tokens;

		ifs.getline(buf, 256, '\n');
		tokens = splitString((std::string)buf, " ");
		if (tokens.empty())
			continue;
		else if (tokens.size() == 2 && tokens[0] == "server" && tokens[1] == "{")
			break;
		else
			throw ServerConf::ConfFail(); // something outside server block
	}
}

void	ServerConf::parseListen( std::vector<std::string> &tokens ) {
	std::cout << "coucou" << std::endl;
}
