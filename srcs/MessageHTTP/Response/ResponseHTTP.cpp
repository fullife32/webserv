/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/30 10:28:19 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
#include <unistd.h>

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
		: m_headerFields(),
		m_requestLine(),
		m_method(0),
		m_header(),
		m_body(),
		m_bufferToSend(NULL)
	{}


	ResponseHTTP::ResponseHTTP(const RequestHTTP & request)
		: m_headerFields(),
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
			m_headerFields = other.m_headerFields;
			m_method = other.m_method;
			m_header << other.m_header;
			if (other.m_bufferToSend != NULL)
			{
				m_bufferToSend = new char [strlen(other.m_bufferToSend)];
				strcpy(m_bufferToSend, other.m_bufferToSend);
			}
			else
				m_bufferToSend = NULL;
		}
		return *this;
	}

/* -------------------------------------------------------------------------- */

	void	ResponseHTTP::clear()
	{
		m_method = 0;
		m_requestLine.clear();
		m_headerFields.clear();
		m_body.clear();
		m_header.clear();
		if (m_bufferToSend != NULL)
			delete [] m_bufferToSend;
		m_bufferToSend = NULL;
	}

	size_t		ResponseHTTP::size() 
	{
		size_t len;
	
		m_body.seekg (0, m_body.end);
		len = m_body.tellg();
		m_body.seekg (0, m_body.beg);
		return len;
	}

	const char *	ResponseHTTP::getNextChunk(size_t bufferSize)
	{
		if (m_bufferToSend == NULL)
			m_bufferToSend = new char[bufferSize + 1];
		memset(m_bufferToSend, 0, bufferSize + 1);

		if (m_header.read(m_bufferToSend, bufferSize))
			return m_bufferToSend;

		size_t	len = strlen(m_bufferToSend);

		if (m_body.is_open() && m_body.read(m_bufferToSend + len, bufferSize - len))
			return m_bufferToSend;
		if (strlen(m_bufferToSend) != 0)
			return m_bufferToSend;
		clear();
		return (NULL);
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
		m_formatedResponse(request.getUrl());
	}

	void	ResponseHTTP::buildError(int StatusCode, const std::string & ReasonPhrase)
	{
		clear();
		m_minimalHeaderFields();
		m_requestLine.statusCode = StatusCode;
		m_requestLine.reasonPhrase = ReasonPhrase;
		m_formated_Error(StatusCode, ReasonPhrase);
	}

	/*
		Set the minimals Header Fields needed for an answer.
	*/
	void	ResponseHTTP::m_minimalHeaderFields()
	{
		 m_headerFields["Date"] = getStringTime();
		 m_headerFields["Server"] = SERVER_NAME;
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
	
	
	void	ResponseHTTP::m_formatedResponse(const std::string & url)
	{
		m_header.clear();
		m_formated_StatusLine();
		m_formated_HeaderFields();
		m_openFile_Body(url);
	}

	void	ResponseHTTP::m_formated_StatusLine()
	{
		m_header << START_LINE_HTTP_VERSION << SP << m_requestLine.statusCode << SP << m_requestLine.reasonPhrase << CRLF;
	}

	void	ResponseHTTP::m_formated_HeaderFields()
	{
		for (std::map<std::string, std::string>::iterator it = m_headerFields.begin(); it != m_headerFields.end(); it++)
			m_header << (*it).first << ": " << (*it).second << CRLF;
		m_header << CRLF;
		
	}

	void	ResponseHTTP::m_openFile_Body(const std::string & url)
	{
		std::cout << "OPENFILE " <<  url << std::endl; ///
		try
		{
			m_body.open(url.data(), std::ofstream::binary);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			throw	MessageErrorException(400); ////
		}

		
	}

	void	ResponseHTTP::m_formated_Error(int statusCode, const std::string & ReasonPhrase)
	{
		m_header.clear();
		m_formated_StatusLine();
		m_formated_HeaderFields();
		m_header << "<!DOCTYPE html>" << '\n';
		m_header << "<html lang=\"en\">" << '\n';
		m_header << "<head>" << '\n';
		m_header << "<meta charset=\"UTF-8\">" << '\n';
		m_header << "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">" << '\n';
		m_header << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << '\n';
		m_header << "<title>" << statusCode << " " << ReasonPhrase << "</title>" << '\n';
		m_header << "</head>" << '\n';
		m_header << "<body>" << '\n';
		m_header << "</body>" << '\n';
		m_header << "</html>" << '\n';
		m_header << CRLF << CRLF;
	}

} // end namespace
