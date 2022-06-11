/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/11 12:22:20 by lvirgini         ###   ########.fr       */
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

ResponseHTTP::ResponseHTTP()
	: MessageMethods(), HeaderFields(),
	m_statusLine(),
	m_server(NULL),
	m_method(0),
	m_header(),
	m_body(),
	m_length(0),
	m_isAutoindex(false)
{}


ResponseHTTP::ResponseHTTP(const ServerConf * server)
	: MessageMethods(), HeaderFields(),
	m_statusLine(),
	m_server(server),
	m_method(0),
	m_header(),
	m_body(),
	m_length(0),
	m_isAutoindex(false),
	m_url()
{}


ResponseHTTP::ResponseHTTP(const ResponseHTTP & copy)
	: MessageMethods(), HeaderFields(),
	m_statusLine(copy.m_statusLine),
	m_server(copy.m_server),
	m_method(copy.m_method),
	m_length(copy.m_length),
	m_isAutoindex(copy.m_isAutoindex),
	m_url(copy.m_url)
{
	m_header << copy.m_header;
	m_body << copy.m_body;
}

ResponseHTTP::~ResponseHTTP()
{}


// ResponseHTTP & ResponseHTTP::operator=(const ResponseHTTP & other)
// {
// 	if (this != &other)
// 	{
// 		clear();
// 		m_headerFields = other.m_headerFields;
// 		m_statusLine = other.m_statusLine;
// 		m_server = other.m_server;
// 		m_method = other.m_method;
// 		m_length = other.m_length;

// 		m_header << other.m_header;
// 	}
// 	return *this;
// }

/* -------------------------------------------------------------------------- */

void	ResponseHTTP::clear()
{
	m_method = 0;
	m_length = 0;
	m_headerFields.clear();
	m_statusLine.clear();
	m_statusLine.statusCode = STATUS_OK;
	m_statusLine.reasonPhrase = m_errors[STATUS_OK];
	if (m_body.is_open())
		m_body.close();
	m_body.clear();
	m_header.str("");
	m_header.clear();
	m_isAutoindex = false;
	m_url = URL();
}

size_t		ResponseHTTP::size() 
{
	return (m_length);
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
	m_url = request.getUrl();
	m_set_minimalHeaderFields();
	std::cout << "XXXXXXXXXXXXXXXXXXXX OK XXXXXXXXXXXXXXXXXXX" << std::endl;

	m_method = request.getMethod();
	std::cout << "XXXXXXXXXXXXXXXXXXXX     END    XXXXXXXXXXXXXXXXXXX" << std::endl;
	m_parseMethod(request);

}

/*
	Construit la reponse en fonction de l'erreur rencontree.
*/
void	ResponseHTTP::buildError(int StatusCode, const std::string & ReasonPhrase, const URL & url)
{
	clear();

	m_set_minimalHeaderFields();
	m_statusLine.statusCode = StatusCode;
	m_statusLine.reasonPhrase = ReasonPhrase;
	m_url = url;
	m_formated_Error(m_url);
	debug_print();
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
		case (DELETE) : m_method_DELETE(request); break;
		default :
			throw MessageErrorException(STATUS_NOT_IMPLEMENTED, request.getUrl());
	}
}


void	ResponseHTTP::m_method_GET(const RequestHTTP & request)
{
	std::cout << "in methode GET" << std::endl;
	
	if (request.hasBody())
		throw MessageErrorException(STATUS_BAD_REQUEST);
	if (request.hasQueryString() || m_url.fileExtension == "php")
		// fonction class CGI(ResponseHTTP )
		m_formated_CGI_Response(request);
	else		
		m_formated_Response();

}

void	ResponseHTTP::m_method_POST(const RequestHTTP & request) // TODO: check Content-Type
{
	std::cout << "in methode POST" << std::endl;

	size_t	ContentLenght;

	if (request.hasBody() == false)
		throw MessageErrorException(STATUS_BAD_REQUEST, m_url); // TODO: a checker
	ContentLenght = atoi(request.get_value_headerFields(HF_CONTENT_LENGTH).data());
	if (request.get_value_headerFields(HF_CONTENT_TYPE).empty()) 
		throw MessageErrorException(STATUS_BAD_REQUEST, m_url);// TODO: if no Content type ?
	m_checkBodySize(request.getBodySize(), ContentLenght);

	std::cout << "OK FOR POST " << std::endl;
	
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
	std::cout << "in methode DELETE" << std::endl;
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
	m_statusLine.statusCode = redirection;
	m_statusLine.reasonPhrase = m_errors[redirection];
	set_headerFields(HF_LOCATION, realPath);
	std::cout << "redirection ?????" << std::endl;
}
else
{
	realPath = m_server->getLocationPath(m_url.serverName, m_url.path);
	if (realPath.empty())
		throw MessageErrorException (100);
	if (m_url.filename.empty())
	{
		m_isAutoindex = m_server->isAutoindexOn(m_url.serverName, m_url.path);
		if (m_isAutoindex == true)
			return (realPath);
		index = m_server->getIndex(m_url.serverName, m_url.path); // TODO:
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

if (maxBodySize != 0 && request_bodySize > maxBodySize)
	throw MessageErrorException(STATUS_PAYLOAD_TOO_LARGE, m_url); // TODO:close client
if (ContentLenght != request_bodySize)
	throw MessageErrorException(STATUS_BAD_REQUEST, m_url);
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
		m_body.open(location.data());
	}
	catch(const std::exception& e)  //// TODO: What to do ? 
	{
		std::cerr << e.what() << " DO NOTHING ? " << '\n';
		return false;
	}
	if ( m_body.is_open() == false)
		return false;
	m_setOpenFileBodySize();
	return true;
}


void	ResponseHTTP::m_openFile_Body(const std::string & location)
{

	std::cout << "OPENFILE location = " << location << std::endl;
	if (m_isAutoindex)
		;// m_build_autoIndex(location);
	else
	{
		try
		{
			m_body.open(location.data());
		}
		catch(const std::exception& e)  //// TODO: What to do ? 
		{
			std::cerr << e.what() << '\n';
			throw	MessageErrorException(100);
		}
	}

	std::cout << "is open = " << m_body.is_open() << std::endl;
	if (m_body.is_open() == false)
		throw MessageErrorException(STATUS_NOT_FOUND, m_url);
}

void	ResponseHTTP::m_openFile_CGI()
{
	m_body_CGI = tmpfile(); // TODO check error opening ?
}

void	ResponseHTTP::m_setOpenFileBodySize() // TODO CHECK
{
		size_t	FileSize; // TODO: maybe long ?

		m_body.seekg (0, m_body.end);
		FileSize = m_body.tellg();

		std::cout << "FileSize = " << FileSize << std::endl;
		setContentLength(FileSize == (size_t)-1 ? 0 : FileSize);
		m_body.seekg (0, m_body.beg);
}

