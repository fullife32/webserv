/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 11:24:35 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/30 08:41:35 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEHTTP_HPP
# define MESSAGEHTTP_HPP

# define SERVER_NAME		"Webserv"
# define START_LINE_HTTP_VERSION "HTTP/1.1"
# define NEWLINE			"\n"
# define EMPTY_LINE			"\n\n"
# define CRLF				"\r\n"
# define SP					" "

# define METHOD_GET			"GET"
# define METHOD_POST		"POST"	
# define METHOD_DELETE		"DELETE"
# define METHOD_HEAD		"HEAD"
# define METHOD_PUT			"PUT"
# define METHOD_CONNECT		"CONNECT"
# define METHOD_OPTIONS		"OPTIONS"
# define METHOD_TRACE		"TRACE"



#include <iostream>	// debug

# include <map>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>
# include <memory.h>
# include "Utils.hpp" // debug
# include "MessageStruct.hpp"
# include "ErrorMessage.hpp"
# include "ParseRequest.hpp"

# include "RequestHTTP.hpp"
# include "ResponseHTTP.hpp"



#endif
