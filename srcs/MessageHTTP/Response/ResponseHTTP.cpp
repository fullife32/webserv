/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/14 19:39:41 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
#include "ServerConf.hpp"
#include <unistd.h>

/*
	POUR LES METHOD : 405 NOT ALLOWED que ce soit une methode qui n'est pas autorisee
	ou qu'elle n'existe pas.
*/

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

ResponseHTTP::ResponseHTTP(const ServerConf * server)
	: MessageMethods(), HeaderFields(), ContentTypes(), ErrorMap(),
	m_statusLine(),
	m_server(server),
	m_method(0),
	m_header(),
	m_body(),
	m_body_CGI(NULL),
	m_length(0),
	m_url(),
	m_isAutoindex(false),
	m_is_redirection(false)
{}


ResponseHTTP::ResponseHTTP(const ResponseHTTP & copy)
	: MessageMethods(), HeaderFields(), ContentTypes(), ErrorMap(),
	m_statusLine(copy.m_statusLine),
	m_server(copy.m_server),
	m_method(copy.m_method),
	m_body_CGI(copy.m_body_CGI),
	m_length(copy.m_length),
	m_url(copy.m_url),
	m_isAutoindex(copy.m_isAutoindex),
	m_is_redirection(copy.m_is_redirection)
	// m_body(copy.m_body)
{
	m_headerFields = copy.m_headerFields;
	// m_header << copy.m_header;
	// m_body << copy.m_body;
}

ResponseHTTP::~ResponseHTTP()
{
	clear();
}

/* -------------------------------------------------------------------------- */

void	ResponseHTTP::clear()
{
	m_statusLine.clear();
	m_headerFields.clear();
	m_method = 0;
	m_length = 0;
	m_header.str("");
	m_header.clear();
	if (m_body.is_open())
		m_body.close();
	m_body.clear();
	m_isAutoindex = false;
	m_is_redirection = false;
	m_url = URL();
	if (m_body_CGI != NULL)
	{
		fclose(m_body_CGI);
		m_body_CGI = NULL;
	}
}

size_t		ResponseHTTP::size() 
{
	return (m_length);
}

/* -------------------------------------------------------------------------- */
/*                     Build Response                                         */
/* -------------------------------------------------------------------------- */

/*
	Builds the response according to the request.
	If an error occurs throw MessageError and rebuilds the response according to the error encountered.
*/
void	ResponseHTTP::buildResponse(const RequestHTTP & request)
{
	clear();
	m_url = request.getUrl();
	m_method = request.getMethod();
	m_set_minimalHeaderFields();
	m_parseMethod(request);
}

/*
	Builds the response according to the error encountered.
*/
void	ResponseHTTP::buildError(int StatusCode, const std::string & ReasonPhrase, const URL & url)
{
	clear();
	m_set_minimalHeaderFields();
	m_statusLine.statusCode = StatusCode;
	m_statusLine.reasonPhrase = ReasonPhrase;
	m_url = url;
	m_formated_Error(m_url);
}

/* -------------------------------------------------------------------------- */
/*                     Method Parser                                          */
/* -------------------------------------------------------------------------- */

/*
	Setup the Method and Parse to the corresponding functions
	throw an exception if Method is not implemented
*/
void	ResponseHTTP::m_parseMethod(const RequestHTTP & request)
{
	switch (m_method)
	{
		case (GET) : m_method_GET(request); break;
		case (POST) : m_method_POST(request); break;
		case (DELETE) : m_method_DELETE(request); break;
		default :
			throw MessageErrorException(STATUS_NOT_IMPLEMENTED, request.getUrl());
	}
}


void	ResponseHTTP::m_method_GET(const RequestHTTP & request)
{
	if (request.hasBody())
		throw MessageErrorException(STATUS_BAD_REQUEST, m_url);
	if (request.hasQueryString() || m_url.fileExtension == "php")
		m_formated_CGI_Response(request);
	else		
		m_formated_Response();

}

void	ResponseHTTP::m_method_POST(const RequestHTTP & request) 
{
	std::cout << "in methode POST" << std::endl; // TODO DEBUG

	if (request.hasBody() == false)
		throw MessageErrorException(STATUS_BAD_REQUEST, m_url);
	size_t	contentLenght = convertStringToSize(request.get_value_headerFields(HF_CONTENT_LENGTH));
	// if (contentLenght != request.getBodySize()) // TODO what to do
	// 	throw MessageErrorException(100, m_url);
	if (request.get_value_headerFields(HF_CONTENT_TYPE).empty())
		throw MessageErrorException(100); // TODO: check Content-Type jamais envoyé par firefox....
	m_checkBodySize(request.getBodySize(), contentLenght);
	m_formated_CGI_Response(request);

	std::cout << "OK FOR POST " << std::endl; // TODO DEBUG
}

/*
	DELETE METHOD:

		200 OK - If the ressource was found and delete from the server.
		204 Accepted - If the ressource wasn't found or already deleted from the
	server.
		If the ressource wasn't found, the client should not get a 404 status,
	for two reasons:
		- It would send an unimportant information about the status of the
	ressource, that the client wanted to delete, to the client.
		- If the ressource was already deleted or doesn't exists, the DELETE
	method works as excepted because the ressource is not present on the server
	anymore.
*/

void	ResponseHTTP::m_method_DELETE(const RequestHTTP & request)
{
	std::stringstream	body;

	if (request.hasBody())
		throw MessageErrorException(STATUS_BAD_REQUEST, m_url);
	if (m_server->isMethodAllowed(m_url.serverName, m_url.path, m_method) == false)
		throw MessageErrorException(STATUS_METHOD_NOT_ALLOWED, m_url);

	std::string	deleteFile = m_foundLocation();

	int		ret = std::remove(deleteFile.c_str());
	bool	success = !std::ifstream(deleteFile.c_str());

	if (ret == -1 || success == false) {
		m_statusLine.statusCode = STATUS_ACCEPTED;
		m_statusLine.reasonPhrase = m_errors[STATUS_ACCEPTED];
	}
	else {
		body << "<!DOCTYPE html>" << CRLF;
		body << "<html>" << CRLF;
		body << "<body>" << CRLF;
		body << "<h1>Ressource deleted.</h1>" << CRLF;
		body << "</body>" << CRLF;
		body << "</html>" << CRLF;
		body << CRLF << CRLF;

		setContentLength(body.str().size());
		set_headerFields(HF_CONTENT_TYPE, "text/html");
	}
	m_formated_StatusLine();
	m_formated_HeaderFields();
	m_header << body.str();
}



/* -------------------------------------------------------------------------- */
/*                     Find Location                                          */
/* -------------------------------------------------------------------------- */



std::string			ResponseHTTP::m_foundLocation()
{
	std::string		formatedPath = m_url.formatedPath();
	std::string		realPath;
	std::string		index;
	int				redirection;

	if (m_server->isMethodAllowed(m_url.serverName, formatedPath, m_method) == false)
		throw MessageErrorException(STATUS_METHOD_NOT_ALLOWED, m_url);
	redirection = m_server->isRedirecting(m_url.serverName, m_url.path, realPath);
	if (redirection != 0)
	{
		m_is_redirection = true;
		m_statusLine.statusCode = redirection;
		m_statusLine.reasonPhrase = m_errors[redirection];
		set_headerFields(HF_LOCATION, realPath);
	}
	else
	{
		realPath = m_server->getLocationPath(m_url.serverName, m_url.path);
		if (realPath.empty())
			throw MessageErrorException (STATUS_NOT_FOUND, m_url);
		if (m_url.filename.empty())
		{
			m_isAutoindex = m_server->isAutoindexOn(m_url.serverName, m_url.path);
			if (m_isAutoindex == true)
				return (realPath);
			index = m_server->getIndex(m_url.serverName, m_url.path);
			if (index.empty())
					throw MessageErrorException(STATUS_FORBIDDEN, m_url);
			realPath = index;
		}
		else 
		{
			realPath += m_url.filename;
		}
	}
	return (realPath);
}


void		ResponseHTTP::m_checkBodySize(size_t request_bodySize, size_t ContentLenght)
{
	if (ContentLenght == 0)
		throw MessageErrorException(STATUS_LENGHT_REQUIRED, m_url);
		
	size_t	maxBodySize = (m_server->getBodySize(m_url.serverName, m_url.formatedPath()));

	// if (maxBodySize != 0 && request_bodySize > maxBodySize) // TODO what to do
	// 	throw MessageErrorException(STATUS_PAYLOAD_TOO_LARGE, m_url);
	// if (ContentLenght != request_bodySize)
	// 	throw MessageErrorException(STATUS_BAD_REQUEST, m_url);
}

/*
	try opening file from URL.
		throw MessageError
*/
bool	ResponseHTTP::m_openFile_Error(const std::string & location)
{
	if (location.empty())
		return false;
	try
	{
		m_body.open(location.data(), std::ifstream::binary);
	}
	catch(const std::exception& e)  //// TODO: What to do ? 
	{
		return false;
	}
	if ( m_body.is_open() == false)
		return false;
	m_setOpenFileBodySize();
	return true;
}


void	ResponseHTTP::m_openFile_Body(const std::string & location)
{
	try
	{
		m_body.open(location.data(), std::ifstream::binary);
	}
	catch(const std::exception& e)  //// TODO: What to do ? 
	{
		std::cerr << e.what() << '\n';
		throw	MessageErrorException(100);
	}
	if (m_body.is_open() == false)
		throw MessageErrorException(STATUS_NOT_FOUND, m_url);
	std::cout <<" BODY IS OPEN" << location.data() << " " <<  m_body.is_open() << std::endl;
	m_setOpenFileBodySize();
}

void	ResponseHTTP::m_openFile_CGI()
{
	m_body_CGI = tmpfile();
	if (m_body_CGI == NULL)
		throw MessageErrorException(STATUS_INTERNAL_SERVER_ERROR, m_url);
}

void	ResponseHTTP::m_setOpenFileBodySize() // TODO CHECK
{
	size_t	FileSize;

	m_body.seekg (0, std::ios::end);
	FileSize = (m_body.tellg());
	setContentLength(FileSize == (size_t)-1 ? 0 : FileSize);
	m_body.seekg (0, std::ios::beg);
}

void	ResponseHTTP::m_setCGIBodySize()
{
	size_t	FileSize;

	fseek(m_body_CGI, 0, SEEK_END);
	fputs(CRLF, m_body_CGI);
	fputs(CRLF, m_body_CGI);
	FileSize = ftell(m_body_CGI);
	rewind(m_body_CGI);
	m_body_CGI_size = (FileSize == (size_t)-1 ? 0 : FileSize);
}

