/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageStruct.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:36:26 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/20 12:01:44 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGESTRUCT_HPP
# define IMESSAGESTRUCT_HPP

# include "ErrorMessage.hpp"
# include <string>

namespace WS {


	enum RequestMethod
	{
		GET,
		POST,
		DELETE
	};


	/* HTTPversion  ------------------------------------------------ */
	/*
		HTTP version = HTTP-name "/" DIGIT "." DIGIT
		HTTP-name   = %x48.54.54.50 ; "HTTP", case-sensitive
	*/

	struct HTTPversion
	{
		std::string		name;
		int				major_version;
		int				minor_version;

		HTTPversion();
		~HTTPversion();
		
		void	formatedVersion(const std::string & version);
	};


	/* Status Code  ------------------------------------------------ */
	/*
		status-code = 3DIGIT
		voir code erreur potentiellement une std::mqp <int, string> avec toute la liste d'erreur et 
		le reason_phrase correspondante ou ENUM ou define ??
	*/

	struct	StatusCode
	{
		int				code;
		std::string 	*reason_phrase; // pointeur sur std::map<int, sting> status code error ? ou enum de code ?
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
	};


	struct StatusLine : public StartLine
	{
		StatusCode		status;	
	};

} //end namespace

#endif
