/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/03 14:57:27 by lvirgini         ###   ########.fr       */
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
		: m_server(NULL),
		m_requestLine(),
		m_method(0),
		m_header(),
		m_body(),m_length(0)
	{
		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE + 1);
	}


	ResponseHTTP::ResponseHTTP(const RequestHTTP & request)
		: m_requestLine()
	{
		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE + 1);
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
			m_headerFields = other.m_headerFields;
			m_server = other.m_server;
			m_requestLine = other.m_requestLine;
			m_method = other.m_method;
			m_length = other.m_length;
			m_header << other.m_header;
			strcpy(m_buffer, other.m_buffer);
		}
		return *this;
	}

/* -------------------------------------------------------------------------- */

	void	ResponseHTTP::clear()
	{
		m_method = 0;
		m_length = 0;
		m_headerFields.clear();
		m_requestLine.clear();
		m_headerFields.clear();
		m_body.clear();
		m_header.clear();
		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE + 1);
		m_server = NULL;
	}

	size_t		ResponseHTTP::size() 
	{
		return (m_length);
	}

	/*
		Prepare le buffer pour envoi a send()
			envoi d'abord le header puis le body de la page
			clear et envoi NULL une fois tout envoyé.
	*/
	const char *	ResponseHTTP::getNextChunk(size_t bufferSize)
	{
		memset(m_buffer, 0, MESSAGE_BUFFER_SIZE);

		if (m_header.read(m_buffer, bufferSize))
			return m_buffer;

		size_t	len = strlen(m_buffer);

		if (m_body.is_open() && m_body.read(m_buffer + len, bufferSize - len))
			return m_buffer;
		if (strlen(m_buffer) != 0)
			return m_buffer;
		clear();
		return (NULL);
	}

/* -------------------------------------------------------------------------- */
/*                     Get PSet                                               */
/* -------------------------------------------------------------------------- */

	/*
		Set Content-Lenght headerFields and keep size in intern variable
	*/
	void	ResponseHTTP::setContentLength(size_t size)
	{
		std::stringstream StringSize;

		StringSize << size;
		m_length = size;
		set_headerFields("Content-Length", StringSize.str()); 
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
		m_set_minimalHeaderFields();
		m_method = request.getMethod();
		m_parseMethod(request);
		if (request.hasQueryString() == true)
			m_formated_CGI_Response(request);
		else		
			m_formated_Response(request.getUrl());
	}

	/*
		Construit la reponse en fonction de l'erreur rencontree.
	*/
	void	ResponseHTTP::buildError(int StatusCode, const std::string & ReasonPhrase)
	{
		clear();
		m_set_minimalHeaderFields();
		m_requestLine.statusCode = StatusCode;
		m_requestLine.reasonPhrase = ReasonPhrase;
		m_formated_Error();
	}

	/*
		Set the minimals Header Fields needed for an answer.
			Date and Server
	*/
	void	ResponseHTTP::m_set_minimalHeaderFields()
	{
		set_headerFields("Date", getStringTime());
		set_headerFields("Server", SERVER_NAME);
	}


/* -------------------------------------------------------------------------- */
/*                     Method Parser                                          */
/* -------------------------------------------------------------------------- */

	/*
		Setup the Method and Parse to the corresponding functions
		throw an exception if Method is not Allowed
	*/
	void	ResponseHTTP::m_parseMethod(const RequestHTTP & request)
	{
		switch (m_method)
		{
			case (GET) : m_method_GET(request); break;
			case (POST) : m_method_POST(request); break;
			case (DELETE) : m_method_DELETE(); break;
			default :
				throw MessageErrorException(STATUS_NOT_IMPLEMENTED);
		}
	}


	void	ResponseHTTP::m_method_GET(const RequestHTTP & request)
	{
		std::cout << "in methode GET" << std::endl;

		if (request.hasBody())
			throw MessageErrorException(STATUS_BAD_REQUEST);
		// TODO: 411 ERROR
	}

	void	ResponseHTTP::m_method_POST(const RequestHTTP & request)
	{
		std::cout << "in methode POST" << std::endl;

		size_t	ContentLenght;
		
		if (request.hasBody() == false)
			throw MessageErrorException(STATUS_BAD_REQUEST); // TODO: a checker
		ContentLenght = atoi(request.get_value_headerFields("Content-Lenght").data());
		if (ContentLenght == 0)
			throw MessageErrorException(STATUS_LENGHT_REQUIRED);
		
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


	void	ResponseHTTP::m_formated_CGI_Response(const RequestHTTP & request)
	{

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
			m_body.open(url.data(), std::fstream::binary);
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
		// std::string			ErrorUrl = m_server->getErrorPage();

		m_header.clear();
		m_formated_StatusLine();
		// if ErrorUrl.empty()
		// {
			m_formated_ErrorBody(body);

		// }
		// else
		// 	m_openFile_Body(ErrorUrl);
		m_formated_HeaderFields();

		m_header << body.str();
	}


	void	ResponseHTTP::m_formated_ErrorBody(std::stringstream & body)
	{
		// if (ErrorUrl.empty())

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
		set_headerFields("Content-Type", "text/html");
	}

/* -------------------------------------------------------------------------- */
/*                     Find Location                                          */
/* -------------------------------------------------------------------------- */



void		ResponseHTTP::m_foundLocation()
{


	// if (server->allowedMethod(m_method) == false)
	// 	throw MessageErrorException(STATUS_METHOD_NOT_ALLOWED);
	
	// 	server.isFound()
}



std::map< std::string, std::string> m_listContentType;


void	m_init_listContentType()
{

	m_listContentType[".aac"] = "audio/aac";
	m_listContentType[".avi"] = "video/x-msvideo";
	m_listContentType[".bmp"] = "image/bmp";
	m_listContentType[".css"] = "text/css";
	m_listContentType[".csv"] = "text/csv";
	m_listContentType[".gif"] = "image/gif";
	m_listContentType[".htm"] = "text/html";
	m_listContentType[".html"] = "text/html";
	m_listContentType[".ico"] = "image/x-icon";
	m_listContentType[".jpeg"] = "image/jpeg";
	m_listContentType[".jpg"] = "image/jpeg";
	m_listContentType[".mpeg"] = "video/mpeg";
	m_listContentType[".png"] = "image/png";
	m_listContentType[".svg"] = "image/svg+xml";
}

} // end namespace
