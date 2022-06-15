/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageStruct.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:37:45 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/14 23:18:50 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
#include <iostream>
#include <cstdlib>

/* HTTPversion  ------------------------------------------------ */

HTTPversion::HTTPversion()
: name(), major_version(), minor_version()
{}

HTTPversion::~HTTPversion()
{}

/*
	HTTP version = HTTP-name "/" major_version "." minor_version
*/

void	HTTPversion::formatedVersion(const std::string & version)
{
	if (version.empty() == true)
		return ;
		
	size_t	found_slash = version.find("/");
	size_t	found_point = version.find(".");

	if (found_slash == std::string::npos || found_point == std::string::npos)
		throw	MessageErrorException(STATUS_BAD_REQUEST);
	name = std::string(&version[0], &version[found_slash]);
	major_version = atoi(version.data() + found_slash + 1);
	minor_version = atoi(version.data() + found_point + 1);
	if (isSupportedVersion() == false)
		throw	MessageErrorException(STATUS_HTTPVERSION_NOT_SUPPORTED);
}

bool	HTTPversion::isSupportedVersion()
{
	if (major_version == 1 && (minor_version == 0 || minor_version == 1))
		return true;
	return false;
}

void	HTTPversion::clear()
{
	name.clear();
	major_version = 0;
	minor_version = 0;
}


/* URL        ------------------------------------------------ */


URL::URL(const URL & copy)
{
	serverName = copy.serverName;
	port = copy.port;
	path = copy.path;
	filename = copy.filename;
	fileExtension = copy.fileExtension;
	pathInfo = copy.pathInfo;
	query = copy.query;
	fragment = copy.fragment;
}	

URL & URL::operator=(const URL & other)
{
	if (this != &other)
	{
		serverName = other.serverName;
		port = other.port;
		path = other.path;
		filename = other.filename;
		fileExtension = other.fileExtension;
		pathInfo = other.pathInfo;
		query = other.query;
		fragment = other.fragment;
	}
	return *this;
}	

void	URL::clear()
{
	serverName.clear();
	path.clear();
	filename.clear();
	fileExtension.clear();
	query.clear();
	fragment.clear();
}

std::string		URL::formatedPath() const
{

	std::string result = path;

	if (filename.empty())
		return (result);
	return (result + "/" + filename);
}


/* Start Line ------------------------------------------------ */

void	RequestLine::clear()
{
	version.clear();
	url.clear();
	method.clear();
}

void	StatusLine::clear()
{
	version.clear();
	statusCode = STATUS_OK;
	reasonPhrase = S_STATUS_OK;
}

/* Header Fields  ------------------------------------------------ */

void		HeaderFields::clear()
{
	m_headerFields.clear();
}

void		HeaderFields::set_headerFields(const HeaderFields::value_type & headerFields)
{
	m_headerFields = headerFields;
}

void	HeaderFields::set_headerFields(const std::string & headerField, const std::string & value)
{
	if (value.empty())
		return ;
	m_headerFields[headerField] = value;
}

HeaderFields const		&HeaderFields::get_headerFields() const
{
	return *this;
}

std::string const		HeaderFields::get_value_headerFields(const std::string & key) const
{
	HeaderFields::value_type::const_iterator found = m_headerFields.find(key);

	if (found == m_headerFields.end())
		return (std::string());
	return (*found).second;
}

/* Message Methods static map  --------------------------------- */

std::map <std::string, int>		init_map_method()
{
	std::map<std::string, int>	methods;

	methods[METHOD_GET]		= GET;
	methods[METHOD_POST]	= POST;	
	methods[METHOD_DELETE]	= DELETE;
	methods[METHOD_HEAD]	= HEAD;
	methods[METHOD_PUT]		= PUT;
	methods[METHOD_CONNECT]	= CONNECT;
	methods[METHOD_OPTIONS]	= OPTIONS;
	methods[METHOD_TRACE]	= TRACE;

	return methods;
}

std::map <std::string, int>		MessageMethods::m_methods = init_map_method();


/* Content Type  ------------------------------------------------ */

std::map< std::string, std::string>		init_map_ContentType()
{
	std::map< std::string, std::string>	map_contentType;

	map_contentType["aac"] = "audio/aac";
	map_contentType["avi"] = "video/x-msvideo";
	map_contentType["bmp"] = "image/bmp";
	map_contentType["css"] = "text/css";
	map_contentType["csv"] = "text/csv";
	map_contentType["gif"] = "image/gif";
	map_contentType["htm"] = "text/html";
	map_contentType["html"] = "text/html";
	map_contentType["ico"] = "image/x-icon";
	map_contentType["jpeg"] = "image/jpeg";
	map_contentType["jpg"] = "image/jpeg";
	map_contentType["js"] = "application/javascript";
	map_contentType["mp3"] = "audio/mp3";
	map_contentType["mpeg"] = "video/mpeg";
	map_contentType["png"] = "image/png";
	map_contentType["svg"] = "image/svg+xml";
	map_contentType["txt"] = "text/txt";
	map_contentType["cpp"] = "text/cpp";
	map_contentType["hpp"] = "text/hpp";
	map_contentType["php"] = "text/php";

	return (map_contentType);
}

std::string ContentTypes::get_contentType(const std::string & requestContentType) const
{
	std::map<std::string, std::string>::const_iterator	found = m_listContentType.find(requestContentType);

	if (found != m_listContentType.end())
		return found->second;
	return (std::string());
}

std::map< std::string, std::string> ContentTypes::m_listContentType = init_map_ContentType();
