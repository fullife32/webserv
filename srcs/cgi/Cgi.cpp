/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:16:17 by rotrojan          #+#    #+#             */
/*   Updated: 2022/06/13 18:52:43 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi
(HeaderFields const &header_fields, ResponseHTTP const &response_http,
 ServerConf const &server_conf) {
	// build environment
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
	env_map["SCRIPT_NAME"] = response_http.get_formatedPath();
	env_map["SERVER_NAME"] = response_http.get_serverName();
	env_map["SERVER_PORT"] = server_conf.getPort();
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["SERVER_SOFTWARE"] = "Webserv/1";
	this->_alloc_env(env_map);
	// build args
	this->_argv = new char*[3];
	std::string arg0 = server_conf.getCgiPath(response_http.get_serverName(), response_http.get_path(), ".php");
	char pathwd[PATH_MAX] ;
	getcwd(pathwd, PATH_MAX);
	std::string arg1 = std::string(pathwd)
		+ "/" + server_conf.getLocationPath(response_http.get_serverName(), response_http.get_path())
		+ response_http.get_fileName();
	// free(tmp);
	this->_argv[0] = new char[arg0.length() + 1];
	std::strcpy(this->_argv[0], arg0.c_str());
	this->_argv[1] = new char[arg1.length() + 1];
	std::strcpy(this->_argv[1], arg1.c_str());
	this->_argv[2] = NULL;
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
	for (int i = 0; this->_argv[i] != NULL; i++)
		delete [] this->_argv[i];
	delete [] this->_argv;
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

void Cgi::execute(int const fd_in, int const fd_out) {

	for (int i = 0; this->_argv[i] != NULL; i++) {
		std::cout << "argv[" << i << "] = " << this->_argv[i] << std::endl;
	}
	pid_t pid = fork();
	if (pid == -1)
		throw Cgi::CgiError(strerror(errno));
	else if (pid == 0) { // child process
		std::cout << "child" << std::endl;
		if (fd_in != -1) {
			if (dup2(fd_in, STDIN_FILENO) == -1)
				throw Cgi::CgiError(strerror(errno));
		}
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			throw Cgi::CgiError(strerror(errno));
		execve(this->_argv[0], this->_argv, this->_env);
		throw Cgi::CgiError(strerror(errno));
	} else { // parent process
// // response_http.m_body -> open -> write -> 
		std::cout << "parent" << std::endl;
	}
}

// Error management

Cgi::CgiError::CgiError(char const *msg): _msg(msg) {
}

char const *Cgi::CgiError::what(void) const throw() {
	return (this->_msg);
}
