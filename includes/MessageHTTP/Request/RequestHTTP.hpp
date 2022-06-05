/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/05 14:58:55 by lvirgini         ###   ########.fr       */
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
		RequestLine							m_requestLine;
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
		URL				getUrl() const;
		void			setRequestLine(const RequestLine & requestLine);
		void			setBody(const std::string & body);

	/* functions    ------------------------------------------------ */
		void			buildRequest();
		void			append(const char * buffer);

		bool			hasQueryString() const ;
		bool			hasBody() const ;


		
		// debug
		 void	debug_print()
		{

			std::cout << " REQUEST DEBUG PRINT ************************************" << std::endl;
			std::cout << "HEADER: " <<  std::endl  << std::endl;
			std::cout << "BODY: " <<  std::endl << m_body << std::endl;
			std::cout << "REQUESTLINE: " <<  std::endl;
			std::cout << "	method: " << m_requestLine.method << std::endl;
			std::cout << "	version: " << m_requestLine.version.name <<  m_requestLine.version.major_version << "." <<  m_requestLine.version.minor_version << std::endl;
			std::cout << "	servername: " << m_requestLine.url.serverName << std::endl;
			std::cout << "	path: " << m_requestLine.url.path << std::endl;
			std::cout << "	filename: " << m_requestLine.url.filename << std::endl;
			std::cout << "	extension: " << m_requestLine.url.fileExtension << std::endl;
			std::cout << "	query: " << m_requestLine.url.query << std::endl;
			std::cout << "	fragment: " << m_requestLine.url.fragment << std::endl;
			std::cout << std::endl;
		}


}; // end class RequestHTTP


} // end namespace

#endif
