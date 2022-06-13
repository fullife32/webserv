/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP_get_set.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 17:51:17 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/13 18:54:57 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
#include "ServerConf.hpp"
#include <unistd.h>

/* -------------------------------------------------------------------------- */
/*                     Get Set                                               */
/* -------------------------------------------------------------------------- */

/*
	Prepare le buffer pour envoi a send()
		envoi d'abord le header puis le body de la page
		clear et envoi NULL une fois tout envoyé.
*/
size_t	ResponseHTTP::getNextChunk(char * buffer)
{
	size_t	len = 0;

	m_header.get(buffer, MESSAGE_BUFFER_SIZE + 1, 0);
	if (m_header.gcount() == MESSAGE_BUFFER_SIZE)
		return MESSAGE_BUFFER_SIZE;
	if (m_body.good())
	{
		len = strlen(buffer);
		len += m_body.readsome(buffer + len, MESSAGE_BUFFER_SIZE - len);
		return (len);
	}
	else if (m_body_CGI != NULL)
	{
		len = strlen(buffer);
		if (fgets(buffer + len, MESSAGE_BUFFER_SIZE - len, m_body_CGI) == NULL)
			clear(); 
		// 	TODO// len + 1 ? checker car fstream prends +1" 
		// check car  Reading stops after an EOF or a newline.""
		return (strlen(buffer));
	}
	len = strlen(buffer);
	if (len == 0)
		clear();
	return (len);
}


void	ResponseHTTP::setContentType(std::string const & extension)
{
	if (extension.empty())
		return ;
	std::map<std::string, std::string>::iterator	found = m_listContentType.find(extension);
	if (found == m_listContentType.end())
		throw MessageErrorException(STATUS_UNSUPPORTED_MEDIA_TYPE, m_url);
	set_headerFields(HF_CONTENT_TYPE, found->second);
}

const URL & ResponseHTTP::get_url() const
{
	return m_url;
}

std::string ResponseHTTP::get_queryString() const
{
	return m_url.query;
}

std::string ResponseHTTP::get_pathInfo() const
{
	return m_url.pathInfo;
}

int			ResponseHTTP::get_method() const
{
	return m_method;
}

std::string ResponseHTTP::get_serverName() const
{
	return m_url.serverName;
}

std::string ResponseHTTP::get_formatedPath() const
{
	return m_url.formatedPath();
}

std::string ResponseHTTP::get_fileName() const
{
	return m_url.filename;
}

std::string ResponseHTTP::get_path() const
{
	return m_url.path;
}

std::string ResponseHTTP::get_port() const
{
	return m_url.port ;
}

/*
	Set the minimals Header Fields needed for an answer.
		Date and Server
*/
void	ResponseHTTP::m_set_minimalHeaderFields()
{
	set_headerFields(HF_DATE, getStringTime());
	set_headerFields(HF_SERVER, SERVER_NAME);
	setContentType(m_url.fileExtension);
}

/*
	Set Content-Lenght headerFields and keep size in intern variable
*/
void	ResponseHTTP::setContentLength(size_t size)
{
	std::stringstream StringSize;

	StringSize << size;
	m_length = size;
	set_headerFields(HF_CONTENT_LENGTH, StringSize.str());
}

