/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservDefine.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:27:56 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/12 11:08:26 by lvirgini         ###   ########.fr       */
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
	
# define MESSAGE_BUFFER_SIZE	256
# define SERVER_NAME				"Webserv"

/*
** Request and Response
*/

# define NEWLINE			"\r\n"
# define CRLF				"\r\n"
# define EMPTY_LINE			"\r\n\r\n" 
# define SP					" "

# define REQUEST_URL_MAX_SIZE		256 		// TODO chooze a max size
# define REQUEST_HEADER_MAX_SIZE	(size_t)-1	// TODO choose a max size
# define START_LINE_HTTP_VERSION	"HTTP/1.1"

#endif
