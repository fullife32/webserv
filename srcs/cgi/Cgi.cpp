/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:16:17 by rotrojan          #+#    #+#             */
/*   Updated: 2022/06/15 15:00:17 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi
(HeaderFields const &header_fields, ResponseHTTP const &response_http,
 ServerConf const &server_conf) {
	// build environment
	std::map<std::string, std::string> env_map;
	char pathwd[PATH_MAX] ;
	getcwd(pathwd, PATH_MAX);

	std::string arg0 = server_conf.getCgiPath(response_http.get_serverName(), response_http.get_path(), ".php");
	std::string arg1 = std::string(pathwd)
		+ "/" + server_conf.getLocationPath(response_http.get_serverName(), response_http.get_path())
		+ response_http.get_fileName();

	env_map["AUTH_TYPE"] = "";
	env_map["CLIENT_MAX_BODY_SIZE"] = convertSizeToString(server_conf.getBodySize(response_http.get_serverName(), response_http.get_path()));
	env_map["CONTENT_LENGTH"] = header_fields.get_value_headerFields(HF_CONTENT_LENGTH);
	env_map["CONTENT_TYPE"] = header_fields.get_value_headerFields(HF_CONTENT_TYPE);
	env_map["DOCUMENT_ROOT"] = server_conf.getLocationPath(response_http.get_serverName(), response_http.get_path());
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["PATH_INFO"] = response_http.get_formatedPath();
	env_map["PATH_TRANSLATED"] = server_conf.getLocationPath(response_http.get_serverName(), response_http.get_path()) + response_http.get_fileName();
	env_map["QUERY_STRING"] = response_http.get_queryString();
	env_map["REDIRECT_STATUS"] = "200";
	env_map["REMOTE_ADDR"] = server_conf.getIp();
	env_map["REMOTE_HOST"] = "";
	env_map["REMOTE_IDENT"] = "";
	env_map["REMOTE_USER"] = "";
	env_map["REQUEST_METHOD"] = response_http.get_method();
	env_map["REQUEST_URI"] = response_http.get_formatedPath();
	env_map["SCRIPT_FILENAME"] = server_conf.getLocationPath(response_http.get_serverName(), response_http.get_path()) + response_http.get_fileName();
	env_map["SCRIPT_NAME"] = response_http.get_formatedPath();
	env_map["SERVER_NAME"] = response_http.get_serverName();
	env_map["SERVER_PORT"] = response_http.get_port();
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["SERVER_SOFTWARE"] = "Webserv/1.0";
	env_map["UPLOAD_STORE"] = server_conf.getUploadPath(response_http.get_serverName(), response_http.get_path());

	this->_alloc_env(env_map);
	// build args
	this->_argv = new char*[3];
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

	pid_t pid = fork();
	if (pid == -1)
		throw Cgi::CgiError(strerror(errno));
	else if (pid == 0) { // child process

		if (fd_in != -1) {
			if (dup2(fd_in, STDIN_FILENO) == -1)
				throw Cgi::CgiError(strerror(errno));
		}
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			throw Cgi::CgiError(strerror(errno));
		execve(this->_argv[0], this->_argv, this->_env);
		exit(errno);
	} else { // parent process
		int	wstatus;

		waitpid(pid, &wstatus, 0);

		if (WIFEXITED(wstatus) == false)
			throw MessageErrorException(STATUS_GATEWAY_TIMEOUT);
		else if (WEXITSTATUS(wstatus) == 2 || WEXITSTATUS(wstatus) == 255)
			throw MessageErrorException(STATUS_NOT_FOUND);
		else if (WEXITSTATUS(wstatus) != 0) 
			throw MessageErrorException(STATUS_INTERNAL_SERVER_ERROR);
	}
}

// Error management

Cgi::CgiError::CgiError(char const *msg): _msg(msg) {
}

char const *Cgi::CgiError::what(void) const throw() {
	return (this->_msg);
}
