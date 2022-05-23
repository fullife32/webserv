/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/23 15:28:09 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include "AMessageHTTP.hpp"
#include "ParseRequest.hpp"
#include "Utils.hpp"

#include <iostream>

namespace WS {


class ParseRequest;

class RequestHTTP : public AMessageHTTP
{
	/*
		protected variables herited from AMessageHTTP:

		StartLine								m_startLine;
		std::map <std::string, std::string> 	m_header_fields;
		std::string								m_body;
		static	std::map <std::string, int>		m_methods;
	*/

	private:
		RequestLine							m_startLine;

	public:

	/* constructor ------------------------------------------------ */
		RequestHTTP();
		RequestHTTP(const RequestHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~RequestHTTP();

	/* operator    ------------------------------------------------ */
		RequestHTTP &	operator=(const RequestHTTP & other);

	/* get / set    ------------------------------------------------ */
		int				getMethod() const;
		void			setRequestLine(const RequestLine & requestLine);
		void			buildRequest(ParseRequest & parser);
		
		

	/* functions    ------------------------------------------------ */


		// debug
		virtual void	debug_print_startline()
		{
			HTTPversion	*v = &m_startLine.version;

			std::cout << "HTTPversion = " << v->name << " " << v->major_version << "." << v->minor_version << std::endl;
			std::cout << "URL = " << m_startLine.target << std::endl;
			std::cout << "method = " << m_startLine.method << std::endl;
		}


}; // end class RequestHTTP


} // end namespace

#endif
