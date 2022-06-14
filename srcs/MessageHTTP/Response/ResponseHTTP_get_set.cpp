/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP_get_set.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 17:51:17 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/14 23:42:46 by lvirgini         ###   ########.fr       */
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
	if (m_body_CGI != NULL)
	{
		len = strlen(buffer);
		if (fgets(buffer + len, MESSAGE_BUFFER_SIZE - len, m_body_CGI) == NULL)
			clear(); 
		return (strlen(buffer));
	}
	else if (m_body.good())
	{
		len = strlen(buffer);
		len += m_body.readsome(buffer + len, MESSAGE_BUFFER_SIZE - len);
		return (len);
	}
	len = strlen(buffer);
	if (len == 0)
		clear();
	return (len);
}

bool	ResponseHTTP::need_to_read() const
{
	return (m_body.good());
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

std::string		ResponseHTTP::get_method() const
{
	std::string methods[3] = {METHOD_GET, METHOD_POST, METHOD_DELETE};

	return methods[m_method];
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

