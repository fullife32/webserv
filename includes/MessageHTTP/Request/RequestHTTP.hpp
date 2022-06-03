/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/03 13:39:38 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include "MessageHTTP.hpp"

namespace WS {


class RequestHTTP : public MessageMethods, public HeaderFields
{
	/*
		protected variables herited from MessageMethods: list of all Methods
			static	std::map <std::string, int>		m_methods;

		protected variables herited from HeaderFields: list of all Methods
			std::map<std::string, std::string>	m_headerFields;	
	*/
	private:
		RequestLine							m_startLine;
		ParseRequest						m_parseRequest;
		std::string							m_body;

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
		std::string		getUrl() const;
		void			setRequestLine(const RequestLine & requestLine);
		void			setBody(const std::string & body);

	/* functions    ------------------------------------------------ */
		void			buildRequest();
		void			append(const std::string & buffer);

		bool			hasQueryString() const ;
		bool			hasBody() const ;


		
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
