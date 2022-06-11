/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservDefine.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:27:56 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/11 13:00:31 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVDEFINE_HPP
# define WEBSERVDEFINE_HPP

# define METHOD_GET			"GET"
# define METHOD_POST		"POST"	
# define METHOD_DELETE		"DELETE"
# define METHOD_HEAD		"HEAD"
# define METHOD_PUT			"PUT"
# define METHOD_CONNECT		"CONNECT"
# define METHOD_OPTIONS		"OPTIONS"
# define METHOD_TRACE		"TRACE"

	enum e_RequestMethod
	{
		GET = 0,
		POST,
		DELETE,
		HEAD,
		PUT,
		CONNECT,
		OPTIONS,
		TRACE
	};
	
# define REQUEST_URL_MAX_SIZE	256 // TODO: chooze a max size
# define REQUEST_HEADER_MAX_SIZE (size_t)-1 // TODO choose a max size
# define MESSAGE_BUFFER_SIZE	256
# define SERVER_NAME			"Webserv"
# define START_LINE_HTTP_VERSION "HTTP/1.1"
# define NEWLINE			"\r\n"
# define EMPTY_LINE			"\r\n\r\n" 
// # define NEWLINE			"\n" // FOR TEST
// # define EMPTY_LINE			"\n\n" // FOR TEST 
# define CRLF				"\r\n"
# define SP					" "

#endif
