/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/26 17:49:45 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHTTP.hpp"
#include <unistd.h>
#include <fstream>

/*
	POUR LES METHOD : 405 NOT ALLOWED que ce soit une methode qui n'est pas autorisee
	ou qu'elle n'existe pas.


*/

namespace WS
{

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

	ResponseHTTP::ResponseHTTP()
		: AMessageHTTP(),
		m_dataResponse(),
		m_requestLine(),
		m_method(0),
		m_chunk(0)
	{}


	ResponseHTTP::ResponseHTTP(const RequestHTTP & request)
		: AMessageHTTP(),
		m_requestLine()
	{
		buildResponse(request);
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
			clear();
			m_requestLine = other.m_requestLine;
			m_header_fields = other.m_header_fields;
			m_body = other.m_body;
			m_method = other.m_method;
			m_dataResponse <<other.m_dataResponse;
			m_chunk = other.m_chunk;
		}
		return *this;
	}

/* -------------------------------------------------------------------------- */

	void	ResponseHTTP::clear()
	{
		m_chunk = 0;
		m_method = 0;
		m_requestLine.clear();
		m_body.clear();
		m_header_fields.clear();
		m_dataResponse.clear();
		m_data.clear();
	}

	size_t		ResponseHTTP::size() const
	{
		return m_data.size();
	}

	size_t		ResponseHTTP::getNextChunkSize(size_t bufferSize) const
	{
		if (m_chunk > size())
			return bufferSize - (m_chunk - size());
		return bufferSize;
	}


	const char *	ResponseHTTP::getNextChunk(size_t bufferSize)
	{
		memset(buffer, 0, 32); ///

		if (m_dataResponse.read(buffer, bufferSize))
			return (buffer);
		
		size_t	len = strlen(buffer);
		if (body_test.read(buffer + len, bufferSize - len))
			return (buffer);
		if (strlen(buffer) != 0)
			return buffer;
		clear();
		return (NULL);
			
		
		// size_t	len = 0;


		// if ( !m_dataResponse.eofbit())
		// 	len = m_dataResponse.read(buffer, bufferSize);
		






		// m_dataResponse.read(buffer, bufferSize);
		// if (bufferSize > size() && body_test.is_open())
		// {
		// 	buffer = new char [bufferSize + 1];
		// 	body_test.read(buffer, bufferSize);
		// 	m_data.append(buffer);

		// }	
		const char *ptr = m_data.data();
		m_chunk += bufferSize;
		return (ptr);
	}
	
/* -------------------------------------------------------------------------- */
/*                     Build Response                                         */
/* -------------------------------------------------------------------------- */

	void	ResponseHTTP::buildResponse(const RequestHTTP & request)
	{
		clear();
		m_minimalHeaderFields();
		m_method = request.getMethod();
		m_parseMethod();
		m_formatedResponse();
		body_test.open("Error404.html", std::ifstream::in); /// FOR TEST
	}

	void	ResponseHTTP::buildError(int StatusCode, const std::string & ReasonPhrase)
	{
		clear();
		m_minimalHeaderFields();
		m_requestLine.statusCode = StatusCode;
		m_requestLine.reasonPhrase = ReasonPhrase;
		m_formated_Error(StatusCode);
		body_test.open("Error404.html", std::ifstream::in); /// FOR TEST

	}

	/*
		Set the minimals Header Fields needed for an answer.
	*/
	void	ResponseHTTP::m_minimalHeaderFields()
	{
		 m_header_fields["Date"] = getStringTime();
		 m_header_fields["Server"] = SERVER_NAME;
	}


/* -------------------------------------------------------------------------- */
/*                     Method Parser                                          */
/* -------------------------------------------------------------------------- */

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
	
/* -------------------------------------------------------------------------- */
/*                     Formated Response                                      */
/* -------------------------------------------------------------------------- */
	
	
	void	ResponseHTTP::m_formatedResponse()
	{
		m_dataResponse.clear();
		m_formated_StatusLine();
		m_formated_HeaderFields();
		m_formated_body();
		
		m_data = m_dataResponse.str();
	}

	void	ResponseHTTP::m_formated_StatusLine()
	{
		m_dataResponse << START_LINE_HTTP_VERSION << SP << m_requestLine.statusCode << SP << m_requestLine.reasonPhrase << CRLF;
	}

	void	ResponseHTTP::m_formated_HeaderFields()
	{
		for (std::map<std::string, std::string>::iterator it = m_header_fields.begin(); it != m_header_fields.end(); it++)
			m_dataResponse << (*it).first << ": " << (*it).second << CRLF;
	}

	void	ResponseHTTP::m_formated_body()
	{
		if (m_body.empty() == false)
			m_dataResponse << CRLF << m_body; /// IF BODY IS EMPTY NEWLINE aussi ?
		m_dataResponse << CRLF << CRLF;
	}

	void	ResponseHTTP::m_formated_Error(int statusCode)
	{
		// std::ofstream		body ("Error404.html", std::ofstream::binary);

		// std::cout << body. << std::endl;
		m_dataResponse.clear();
		m_formated_StatusLine();
		m_formated_HeaderFields();
		m_dataResponse << CRLF << CRLF;

		// m_dataResponse << body.str();
	}

} // end namespace
