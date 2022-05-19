/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:22:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 11:59:06 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMessageHTTP_HPP
# define AMessageHTTP_HPP

# include "AMessageStruct.hpp"
# include <string>
# include <vector>
# include <map>

#include "Utils.hpp" // debug
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


class AMessageHTTP 
{

	protected:
		StartLine								m_startLine;
		std::map <std::string, std::string> 	m_header_fields;
		std::string								m_body;
		std::map <int, std::string>				m_error;


	public:
		// constructor
		AMessageHTTP() {};
		// AMessageHTTP(const AMessageHTTP & copy);

		// destructor
		virtual ~AMessageHTTP() {};


		// operator
		AMessageHTTP &	operator=(const AMessageHTTP & other);


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


}; // end class AMessageHTTP

} // end namespace


#endif
