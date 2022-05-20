/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:22:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/20 18:22:17 by lvirgini         ###   ########.fr       */
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


class RequestHTTP;



/*
** Interface pour les Request et Response.
*/


class AMessageHTTP 
{

	protected:
		StartLine								m_startLine;
		std::map <std::string, std::string> 	m_header_fields;
		std::string								m_body;
		static	std::map <std::string, int>		m_methods;


	public:
	/* constructor ------------------------------------------------ */
		AMessageHTTP() ;
		AMessageHTTP(const AMessageHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~AMessageHTTP() ;


	/* operator    ------------------------------------------------ */
		AMessageHTTP &	operator=(const AMessageHTTP & other);


	/* set		    ------------------------------------------------ */
		void			setBody(const std::string & body);
		void			setHeaderFields(const std::map<std::string, std::string> & headerFields);


	/* functions    ------------------------------------------------ */


















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


std::map <std::string, int>		init_map_method();

} // end namespace


#endif
