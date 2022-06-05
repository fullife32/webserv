/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/05 17:20:44 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
#include "ServerConf.hpp"
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
		: MessageMethods(), HeaderFields(),
		m_statusLine(),
		m_server(NULL),
		m_method(0),
		m_header(),
		m_body(),
		m_length(0)
	{}


	ResponseHTTP::ResponseHTTP(Server * server)
		: MessageMethods(), HeaderFields(),
		m_statusLine(),
		m_server(server),
		m_method(0),
		m_header(),
		m_body(),
		m_length(0)
	{}


	ResponseHTTP::ResponseHTTP(const ResponseHTTP & copy)
		: MessageMethods(), HeaderFields(),
		m_statusLine(copy.m_statusLine),
		m_server(copy.m_server),
		m_method(copy.m_method),
		m_length(copy.m_length)
	{
		m_header << copy.m_header;
		m_body << copy.m_body;
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
			m_statusLine = other.m_statusLine;
			m_method = other.m_method;
			m_length = other.m_length;
			m_header << other.m_header;
		}
		return *this;
	}

/* -------------------------------------------------------------------------- */

	void	ResponseHTTP::clear()
	{
		m_method = 0;
		m_length = 0;
		m_headerFields.clear();
		m_statusLine.clear();
		if (m_body.is_open())
			m_body.close();
		m_body.clear();
		m_header.str("");
		m_header.clear();
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
	size_t	ResponseHTTP::getNextChunk(char * buffer)
	{
		size_t	len;

		if (m_header.read(buffer, MESSAGE_BUFFER_SIZE))
			return strlen(buffer);

		len = strlen(buffer);

		if (m_body.is_open() && m_body.read(buffer + len, MESSAGE_BUFFER_SIZE - len))
			return strlen(buffer);
			
		len = strlen(buffer);
		if (len == 0)
			clear();
		return (len);
	}

/* -------------------------------------------------------------------------- */
/*                     Get Set                                               */
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

	}

	/*
		Construit la reponse en fonction de l'erreur rencontree.
	*/
	void	ResponseHTTP::buildError(int StatusCode, const std::string & ReasonPhrase)
	{
		clear();
		m_set_minimalHeaderFields();
		m_statusLine.statusCode = StatusCode;
		m_statusLine.reasonPhrase = ReasonPhrase;

		m_formated_Error();
		debug_print();
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
		if (request.hasQueryString())
			m_formated_CGI_Response(request);
		else		
			m_formated_Response(request.getUrl());

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
	void	ResponseHTTP::m_formated_Response(const URL & url)
	{
		m_header.clear();
		m_openFile_Body(url);
		m_formated_StatusLine();
		m_formated_HeaderFields();
	}


	void	ResponseHTTP::m_formated_CGI_Response(const RequestHTTP & request)
	{
		std::cout << "there is a query string in the request" << std::endl;
	}

	void	ResponseHTTP::m_formated_StatusLine()
	{
		m_header << START_LINE_HTTP_VERSION << SP << m_statusLine.statusCode << SP << m_statusLine.reasonPhrase << CRLF;
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
	void	ResponseHTTP::m_openFile_Body(const URL & url)
	{
		int	FileSize; // TODO: maybe long ?

		std::cout << "OPENFILE URL formated = " << url.formatedPath() << std::endl; ///
		
		std::string  location(m_server->getConf().getLocationPath(url.serverName, url.formatedPath()));
		
		// m_body.open("test_config/simple/simple.html");
		try
		{
			m_body.open(location.data());
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			throw	MessageErrorException(400); ////
		}
		std::cout << "is open = " << m_body.is_open() << std::endl;

		// save content length for ifstream
		m_body.seekg (0, m_body.end);
		FileSize = m_body.tellg();

		std::cout << "FileSize = " << FileSize << std::endl;
		setContentLength(FileSize == -1 ? 0 : FileSize);
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

		std::string	background_color;

		if (m_statusLine.statusCode >= 300 && m_statusLine.statusCode < 400)
			background_color = "\"background-color: bisque;";
		else if (m_statusLine.statusCode >= 400 && m_statusLine.statusCode < 500)
			background_color = "\"background-color: coral;";
		else
			background_color = "\"background-color: tomato;";

		body << "<!DOCTYPE html>" << '\n';
		body << "<html lang=\"en\" style= " << background_color << ">\n";
		body << "<head>" << '\n';
		body << "<meta charset=\"UTF-8\">" << '\n';
		body << "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">" << '\n';
		body << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << '\n';
		body << "<title>" << m_statusLine.statusCode << " " << m_statusLine.reasonPhrase << "</title>" << '\n';
		body << "</head>" << '\n';
		body <<  "<body style=\"font-size: xxx-large;font-family: monospace;text-align: -webkit-center; >\"; >" << '\n';
		body << m_statusLine.statusCode << " " << m_statusLine.reasonPhrase << '\n';
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
