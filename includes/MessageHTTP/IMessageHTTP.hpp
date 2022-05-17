/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessageHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:22:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/17 14:50:22 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGEHTTP_HPP
# define IMESSAGEHTTP_HPP

# include <string>
# include <vector>
# include <map>
#include "usefull.hpp"
#include <iostream>

namespace WS {

/*
**	HTTP version = HTTP-name "/" DIGIT "." DIGIT
**	HTTP-name   = %x48.54.54.50 ; "HTTP", case-sensitive
*/

struct HTTPversion
{
	std::string		name;
	int				major_version;
	int				minor_version;

	HTTPversion();
	HTTPversion(std::string	version);
	~HTTPversion();
};

/*
** status-code    = 3DIGIT voir code erreur potentiellement une std::mqp <int, string> avec toute la liste d'erreur et 
** le reason_phrase correspondante ou ENUM ou define ??
*/

struct	StatusCode
{
	int				code;
	std::string 	*reason_phrase; // pointeur sur std::map<int, sting> status code error ? ou enum de code ?
};

/*
** Start line
*/

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


/*
** Interface pour les Request et Response.
*/


class IMessageHTTP 
{

	protected:
		StartLine								m_start_line;
		std::map < std::string, std::string > 	m_header_fields;
		std::string								m_body;


	public:
		// constructor
		IMessageHTTP() {};
		// IMessageHTTP(const IMessageHTTP & copy);

		// destructor
		virtual ~IMessageHTTP() {};


		// operator
		IMessageHTTP &	operator=(const IMessageHTTP & other);


		// functions

		// debug

		
		virtual void	debug_print_startline() = 0;
		void	debug_print_Message()
		{
			debug_print_startline();
			
			std::cout << "Header :" << std::endl;
			// printMap<std::string>(m_header_fields);
			std::map<std::string, std::string>::const_iterator	it;

			for (it = m_header_fields.begin(); it != m_header_fields.end(); it++)
				std::cout << "key=" << (*it).first << " value=" << (*it).second << std::endl;

			std::cout << "Body :" << std::endl;
			std::cout << m_body << std::endl;

		}


}; // end class IMessageHTTP

} // end namespace


#endif
