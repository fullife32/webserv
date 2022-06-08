/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rotrojan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:16:17 by rotrojan          #+#    #+#             */
/*   Updated: 2022/06/08 15:19:01 by rotrojan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi
(HeaderFields const &header_fields, ResponseHTTP const &response_http,
 ServerConf const &server_conf) {
	std::map<std::string, std::string> env_map;
	env_map["AUTH_TYPE"] = ""; // ?
	env_map["CONTENT_LENGTH"] = header_fields.get_value_headerFields(HF_CONTENT_LENGTH);
	env_map["CONTENT_TYPE"] = header_fields.get_value_headerFields(HF_CONTENT_TYPE);
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["PATH_INFO"] = response_http.get_pathInfo();
	// env_map["PATH_TRANSLATED"] = server_conf.getLocationPath(response_http.get_serverName(), response_http.get_path());
	env_map["QUERY_STRING"] = response_http.get_queryString();
	env_map["REMOTE_ADDR"] = server_conf.getIp();
	env_map["REMOTE_HOST"] = ""; // ?
	env_map["REMOTE_IDENT"] = "";// ?
	env_map["REMOTE_USER"] = ""; // ?
	env_map["REQUEST_METHOD"] = response_http.get_method();
	// env_map["SCRIPT_NAME"] = response_http.get_fileName();
	env_map["SERVER_NAME"] = response_http.get_serverName();
	env_map["SERVER_PORT"] = server_conf.getPort();
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["SERVER_SOFTWARE"] = "Webserv";
	this->_env = _alloc_env(env_map);
}

// Cgi(Cgi const &cgi) {
	// int i = 0;
	// for (; this->_env[i] != NULL; ++i)

	// this->_env = cgi._env;
// }
Cgi &Cgi::operator=(Cgi const &rhs) {
	if (&rhs != this) {
		this->_env = rhs._env;
	}
}

Cgi::~Cgi(void) {
	for (char const *ptr = *this->_env; ptr != NULL; ++ptr)
		delete [] ptr;
	delete [] this->_env;
}

char **Cgi::_alloc_env(std::map<std::string, std::string> &env_map) {
	char **env = new char*[env_map.size() + 1];
	env[env_map.size()] = NULL;
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = env_map.begin(), ite = env_map.end(); it != ite; ++it) {
		std::string tmp = it->first + "=" + it->second;
		this->_env[i] = new char[tmp.length() + 1];
		std::strcpy(this->_env[i], tmp.c_str());
	}
	return (env);
}

void Cgi::execute(void) {

	for (char const *ptr = *this->_env; ptr != NULL; ++ptr)
		std::cout << *ptr << std::endl;
	// int fd[2]
	// int ret;
	
	// ret = pipe(fd);
	// if (ret == -1)
		// throw Cgi::CgiError(strerror(errno));
	// pid_t pid = fork();
	// if (pid == -1)
		// throw Cgi::CgiError(strerror(errno));
	// else if (pid == 0) {
		// ret = execve(this->_from_map_to_C_env());
		// if (ret == -1)
			// throw Cgi::CgiError(strerror(errno));
	// } else {
// response_http.m_body -> open -> write -> 
// server_conf.getCgiPath();
	// }
}

Cgi::CgiError::CgiError(char const *msg): _msg(msg) {
}

char const *Cgi::CgiError::what(void) const throw() {
	return (this->_msg);
}
