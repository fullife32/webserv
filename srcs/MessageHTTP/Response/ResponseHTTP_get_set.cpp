/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP_get_set.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 17:51:17 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/07 16:42:31 by lvirgini         ###   ########.fr       */
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




/*
	Set the minimals Header Fields needed for an answer.
		Date and Server
*/
void	ResponseHTTP::m_set_minimalHeaderFields()
{
	set_headerFields(HF_DATE, getStringTime());
	set_headerFields(HF_SERVER, SERVER_NAME);
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


// std::string ResponseHTTP::get_path() const
// {
// 	return m_url.path;
// }
