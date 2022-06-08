/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rotrojan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 17:01:20 by rotrojan          #+#    #+#             */
/*   Updated: 2022/06/08 15:16:09 by rotrojan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP
#include "MessageHTTP.hpp"
#include <sys/types.h>
#include <unistd.h>

class Cgi {

public:
	Cgi(HeaderFields const &header_fields, ResponseHTTP const &response_http,
	 ServerConf const &server_conf);
	// Cannonical
	// Cgi(Cgi const &cgi);
	Cgi &operator=(Cgi const &rhs);
	~Cgi(void);

	// main function
	void execute(void);

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
	char **_alloc_env(std::map<std::string, std::string> &env_map);
	Cgi(void);
};

#endif
