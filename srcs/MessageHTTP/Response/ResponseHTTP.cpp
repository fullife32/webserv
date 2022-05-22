/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/22 18:48:48 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHTTP.hpp"

/*
	POUR LES METHOD : 405 NOT ALLOWED que ce soit une methode qui n'est pas autorisee
	ou qu'elle n'existe pas.


*/

namespace WS
{

	/*
		Construct a response from a requestHTTP
	*/

	ResponseHTTP::ResponseHTTP(const RequestHTTP & request)
		: AMessageHTTP(),
		m_startLine(),
		m_method(request.getMethod())
	{
		m_minimalHeaderFields();
		m_parseMethod();
	}




	ResponseHTTP::ResponseHTTP(const ResponseHTTP & copy)
	{
		*this = copy;
	}

	ResponseHTTP::~ResponseHTTP()
	{}


	ResponseHTTP & ResponseHTTP::operator=(const ResponseHTTP & other)
	{
		if (this != &other)
		{
			m_startLine = other.m_startLine;
			m_header_fields = other.m_header_fields;
			m_body = other.m_body;
			m_method = other.m_method;
			m_dataResponse = other.m_dataResponse;
			m_send = other.m_send;
		}
		return *this;
	}


	void	ResponseHTTP::clear()
	{
		m_send = 0;
		m_method = 0;
		m_startLine.clear();
		m_body.clear();
		m_header_fields.clear();
		m_dataResponse.clear();

	}

	/*
		Set the minimals Header Fields needed for an answer.
	*/
	void	ResponseHTTP::m_minimalHeaderFields()
	{
		 m_header_fields["Date"] = getStringTime();
		 m_header_fields["Server"] = SERVER_NAME;
	}


	/*
		Setup the Method and Parse to the corresponding functions
		throw an exception if Method is not Allowed
	*/
	void	ResponseHTTP::m_parseMethod()
	{
		switch (m_method)
		{
			case (GET) : m_method_GET(); break;
			case (POST) : m_method_POST(); break;
			case (DELETE) : m_method_DELETE(); break;
			default :
				throw MessageErrorException(STATUS_METHOD_NOT_ALLOWED);
		}
	}


	void	ResponseHTTP::m_method_GET()
	{
		std::cout << "in methode GET" << std::endl;
	}
	void	ResponseHTTP::m_method_POST()
	{
		std::cout << "in methode POST" << std::endl;
	}
	void	ResponseHTTP::m_method_DELETE()
	{
		std::cout << "in methode DELETE" << std::endl;
	}


	void	ResponseHTTP::m_formatedResponse()
	{
		m_dataResponse.append(START_LINE_HTTP_VERSION);
		m_dataResponse.append(SP);
		m_dataResponse.append(std::string(m_startLine.status.code));
		m_dataResponse.append(SP);
		m_dataResponse.append(m_startLine.status.reasonPhrase);
	}

	// void	ResponseHTTP::m_buildHeader()
	// {


	// 	// header << START_LINE_HTTP_VERSION << SP << m_startLine.status.code << SP << m_startLine.status.reasonPhrase;
	// }

} // end namespace


