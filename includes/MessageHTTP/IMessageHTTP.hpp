/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessageHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:22:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/18 15:42:16 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGEHTTP_HPP
# define IMESSAGEHTTP_HPP

# include "IMessageStruct.hpp"
# include <string>
# include <vector>
# include <map>

#include "utils.hpp" // debug
#include <iostream>	// debug

namespace WS {

enum RequestMethod
{
	GET,
	POST,
	DELETE
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
		std::map<int, std::string>				m_error;


	public:
		// constructor
		IMessageHTTP() {};
		// IMessageHTTP(const IMessageHTTP & copy);

		// destructor
		virtual ~IMessageHTTP() {};


		// operator
		IMessageHTTP &	operator=(const IMessageHTTP & other);


		// functions
		
		
		//set
		void			setBody(const std::string & body);
		void			setHeaderFields(const std::map<std::string, std::string> & headerFields);


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
