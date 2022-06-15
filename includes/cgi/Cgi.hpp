/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 17:01:20 by rotrojan          #+#    #+#             */
/*   Updated: 2022/06/13 19:05:49 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP
#include "MessageHTTP.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

class Cgi {

public:
	Cgi(HeaderFields const &header_fields, ResponseHTTP const &response_http,
	 ServerConf const &server_conf);
	// Cannonical
	Cgi(Cgi const &cgi);
	Cgi &operator=(Cgi const &rhs);
	~Cgi(void);

	// main function
	void execute(int const fd_int, int const fd_out);

	// error handling
	class CgiError: public std::exception {
	public:
		CgiError(char const *msg);
		char const *what() const throw();
	private:
		char const *_msg;
	};

private:
	// the environment build during construction that will be passed to execve()
	char **_env;
	char **_argv;
	void _alloc_env(std::map<std::string, std::string> &env_map);
	Cgi(void);
};

#endif
