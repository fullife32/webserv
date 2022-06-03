/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageStruct.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:36:26 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/30 09:28:01 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGESTRUCT_HPP
# define MESSAGESTRUCT_HPP

#include "MessageHTTP.hpp"

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


	/* Request Line ------------------------------------------------ */


	struct RequestLine
	{
		HTTPversion		version;
		std::string		target; // URL
		std::string		method; // ACTIONS : GET, POST, DELETE

		void	clear();
	};


	/* Status Line  ------------------------------------------------ */
	/*
		status-code = 3DIGIT
		voir code erreur potentiellement une std::mqp <int, string> avec toute la liste d'erreur et 
		le reasonPhrase correspondante ou ENUM ou define ??
	*/

	struct StatusLine
	{
		HTTPversion		version;
		int				statusCode;
		std::string 	reasonPhrase; // pointeur sur std::map<int, sting> status code error ? ou enum de code ?
	
		void	clear();
	};


	/* Message Methods static map  --------------------------------- */

	class MessageMethods
	{
		protected:
			static	std::map <std::string, int>		m_methods;
	};

std::map <std::string, int>		init_map_method();


} //end namespace

#endif
