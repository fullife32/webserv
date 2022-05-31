/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/31 12:09:40 by lvirgini         ###   ########.fr       */
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
		return (m_length);
	}

	// size_t		ResponseHTTP::size_already_read() // si on utilise Content-range (envoi de plusieurs chunk) ?
	// {

	// }

	/*
		Prepare le buffer pour envoi a send()
			envoi d'abord le header puis le body de la page
			clear et envoi NULL une fois tout envoyé.
	*/
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

	/*
		Construit la reponse en fonction de la requete.
		Si une erreur se produit throw MessageError et reconstruit la reponse en fonction de l'erreur rencontree.
	*/
	void	ResponseHTTP::buildResponse(const RequestHTTP & request)
	{
		clear();
		m_minimalHeaderFields();
		m_method = request.getMethod();
		m_parseMethod();
		m_formated_Response(request.getUrl());
	}

	/*
		Construit la reponse en fonction de l'erreur rencontree.
	*/
	void	ResponseHTTP::buildError(int StatusCode, const std::string & ReasonPhrase)
	{
		clear();
		m_minimalHeaderFields();
		m_requestLine.statusCode = StatusCode;
		m_requestLine.reasonPhrase = ReasonPhrase;
		m_formated_Error();
	}

	/*
		Set the minimals Header Fields needed for an answer.
			Date and Server
	*/
	void	ResponseHTTP::m_minimalHeaderFields()
	{
		setHeaderFields("Date", getStringTime());
		setHeaderFields("Server", SERVER_NAME);
	}

	/*
		Set Content-Lenght headerFields and keep size in intern variable
	*/
	void	ResponseHTTP::setContentLength(size_t size)
	{
		std::stringstream StringSize;

		StringSize << size;
		m_length = size;
		setHeaderFields("Content-Length", StringSize.str()); 
	}

	/*
		Set a headerFields
	*/
	void	ResponseHTTP::setHeaderFields(const std::string & headerField, const std::string & value)
	{
		m_headerFields[headerField] = value;
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
	
	/*
		Formated Response : 
			try openFile from URL
			formated Header : Status Line and Header Fields.
	*/
	void	ResponseHTTP::m_formated_Response(const std::string & url)
	{
		m_header.clear();
		m_openFile_Body(url);
		m_formated_StatusLine();
		m_formated_HeaderFields();
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

	/*
		try opening file from URL.
			throw MessageError
	*/
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


		// save content length for ifstream
		m_body.seekg (0, m_body.end);
		setContentLength(m_body.tellg());
		m_body.seekg (0, m_body.beg);
	}

/* -------------------------------------------------------------------------- */
/*                     Formated Error Response                                */
/* -------------------------------------------------------------------------- */

	/*
		formated Error Response:
			create a simple body html for Error
	*/
	void	ResponseHTTP::m_formated_Error()
	{
		std::stringstream	body;

		m_header.clear();
		m_formated_StatusLine();
		m_formated_ErrorBody(body);
		m_formated_HeaderFields();

		m_header << body.str();
	}


	void	ResponseHTTP::m_formated_ErrorBody(std::stringstream & body)
	{
		body << "<!DOCTYPE html>" << '\n';
		body << "<html lang=\"en\">" << '\n';
		body << "<head>" << '\n';
		body << "<meta charset=\"UTF-8\">" << '\n';
		body << "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">" << '\n';
		body << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << '\n';
		body << "<title>" << m_requestLine.statusCode << " " << m_requestLine.reasonPhrase << "</title>" << '\n';
		body << "</head>" << '\n';
		body << "<body>" << '\n';
		body << "</body>" << '\n';
		body << "</html>" << '\n';
		body << CRLF << CRLF;

		setContentLength(body.str().size());
		setHeaderFields("Content-Type", "text/html");
	}

/* -------------------------------------------------------------------------- */
/*                     Find Location                                          */
/* -------------------------------------------------------------------------- */




} // end namespace
