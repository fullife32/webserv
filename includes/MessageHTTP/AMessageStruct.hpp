/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageStruct.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:36:26 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/25 11:13:14 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGESTRUCT_HPP
# define IMESSAGESTRUCT_HPP

# include "ErrorMessage.hpp"
# include <string>

# define SERVER_NAME		"Webserv"
# define SP					" "
# define START_LINE_HTTP_VERSION "HTTP/1.1"
# define CRLF				"\r\n"
# define NEW_LINE			"\n"

# define METHOD_GET			"GET"
# define METHOD_POST		"POST"	
# define METHOD_DELETE		"DELETE"
# define METHOD_HEAD		"HEAD"
# define METHOD_PUT			"PUT"
# define METHOD_CONNECT		"CONNECT"
# define METHOD_OPTIONS		"OPTIONS"
# define METHOD_TRACE		"TRACE"


namespace WS {


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


	/* HTTPversion  ------------------------------------------------ */
	/*
		HTTP version = HTTP-name "/" DIGIT "." DIGIT
		HTTP-name   = %x48.54.54.50 ; "HTTP", case-sensitive

		isSupportedVersion() is only 1.1 for our Webserv
	*/

	struct HTTPversion
	{
		std::string		name;
		int				major_version;
		int				minor_version;

		HTTPversion();
		~HTTPversion();
		
		void	formatedVersion(const std::string & version);
		bool	isSupportedVersion();
		void	clear();
	};


	/* Status Code  ------------------------------------------------ */
	/*
		status-code = 3DIGIT
		voir code erreur potentiellement une std::mqp <int, string> avec toute la liste d'erreur et 
		le reasonPhrase correspondante ou ENUM ou define ??
	*/

	struct	StatusCode
	{
		int				code;
		std::string 	*reasonPhrase; // pointeur sur std::map<int, sting> status code error ? ou enum de code ?
	};


	/* Start Line ------------------------------------------------ */

	struct StartLine
	{
		HTTPversion		version;

	};


	struct RequestLine : public StartLine
	{
		std::string		target; // URL
		std::string		method; // ACTIONS : GET, POST, DELETE

		void	clear();
	};


	struct StatusLine : public StartLine
	{
		StatusCode		status;	
	
		void	clear();
	};

} //end namespace

#endif
