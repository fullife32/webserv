/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rotrojan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:16:17 by rotrojan          #+#    #+#             */
/*   Updated: 2022/06/08 20:16:30 by rotrojan         ###   ########.fr       */
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
	env_map["PATH_TRANSLATED"] = server_conf.getLocationPath(response_http.get_serverName(), response_http.get_path());
	env_map["QUERY_STRING"] = response_http.get_queryString();
	env_map["REMOTE_ADDR"] = server_conf.getIp();
	env_map["REMOTE_HOST"] = ""; // ?
	env_map["REMOTE_IDENT"] = "";// ?
	env_map["REMOTE_USER"] = ""; // ?
	env_map["REQUEST_METHOD"] = response_http.get_method();
	env_map["SCRIPT_NAME"] = response_http.get_fileName();
	env_map["SERVER_NAME"] = response_http.get_serverName();
	env_map["SERVER_PORT"] = server_conf.getPort();
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["SERVER_SOFTWARE"] = "Webserv";
	this->_alloc_env(env_map);
}

Cgi::Cgi(Cgi const &cgi) {
	*this = cgi;
}

Cgi &Cgi::operator=(Cgi const &rhs) {
	if (&rhs != this) {
		int size_env = 0;
		while (rhs._env[size_env] != NULL)
			++size_env;
		for (int i = 0; rhs._env[i] != NULL; ++i) {
			this->_env[i] = new char[std::strlen(rhs._env[i]) + 1];
			std::strcpy(this->_env[i], rhs._env[i]);
		}
		this->_env[size_env] = NULL;
	}
	return (*this);
}

Cgi::~Cgi(void) {
	for (int i = 0; this->_env[i] != NULL; i++)
		delete [] this->_env[i];
	delete [] this->_env;
}

void Cgi::_alloc_env(std::map<std::string, std::string> &env_map) {
	this->_env = new char *[env_map.size() + 1];
	this->_env[env_map.size()] = NULL;
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = env_map.begin(), ite = env_map.end(); it != ite; ++it) {
		std::string tmp(it->first + "=" + it->second);
		this->_env[i] = new char[tmp.length() + 1];
		std::strcpy(this->_env[i], tmp.c_str());
		++i;
	}
}

void Cgi::execute(void) {

	for (int j = 0; this->_env[j] != NULL; ++j)
		std::cout << this->_env[j] << std::endl;
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
