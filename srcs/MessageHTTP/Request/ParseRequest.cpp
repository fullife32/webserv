/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:48:48 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/30 10:51:48 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"

namespace WS {

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

ParseRequest::ParseRequest()
{}


ParseRequest::ParseRequest(std::string	data)
	: m_data(data)
{
	if (data.empty())
		throw MessageErrorException(STATUS_BAD_REQUEST);
}


ParseRequest::~ParseRequest()
{}

/* -------------------------------------------------------------------------- */


/*
**	append buffer from recv to complete the request data.
*/

std::string &	ParseRequest::append(const std::string & str)
{
	return m_data.append(str);
}


/* -------------------------------------------------------------------------- */

/*
** After completely receive request : formated the requestHTTP
**	separate body and header
** formated the requestHeader in a requestLine
*/

void	ParseRequest::m_prepareRequestBuilding()
{
	std::vector<std::string>	split;

	m_separateHeaderBody();

	split = splitString(m_header, NEWLINE);
	if (split.size() == 0)
		throw MessageErrorException(STATUS_BAD_REQUEST);
	
	m_formated_RequestLine(split[0]);
	split.erase(split.begin());
	m_formated_HeaderFields(split);
	m_check_host_HeaderFields();
}


RequestLine	ParseRequest::getRequestLine()
{
	if (m_header.empty())
		m_prepareRequestBuilding();
	return(m_requestLine);
	
}
std::string	ParseRequest::getBody()
{
	if (m_header.empty())
		m_prepareRequestBuilding();
	return (m_body);
}
		
std::map<std::string, std::string>	ParseRequest::getHeaderFields()
{
	if (m_header.empty())
		m_prepareRequestBuilding();
	return (m_headerFields);
}

/* -------------------------------------------------------------------------- */


/*
** Separate Header from Body
		find separation empty line between header and body request
		it can be the end of file and body will be empty

		throw exception if empty line is not found (400: Bad Request)
*/

void			ParseRequest::m_separateHeaderBody()
{
	size_t		separation = m_data.find(EMPTY_LINE);
	
	if (separation == std::string::npos)
		throw MessageErrorException(STATUS_BAD_REQUEST);
	m_body =   std::string(&m_data[separation + 2], &m_data[m_data.size()]);
	m_header = std::string(&m_data[0], &m_data[separation]);
}

/*
	 get first line of Header
		formated the first line of Request Header :
		Method SP URL SP HTTPversion

		for now HTTPversion is facultatif
		throw exception (400 Bad Request)
*/
void 		ParseRequest::m_formated_RequestLine(const std::string & startline)
{
	std::vector<std::string>	split;
	
	split = splitString(startline, " ");
	if (split.size() < 2 || split.size() > 3)
		throw MessageErrorException(STATUS_BAD_REQUEST);

	m_requestLine.method = split[0];
	m_requestLine.target = split[1];
	if (split.size() == 3)
		m_requestLine.version.formatedVersion(split[2]);
}

/*
**	get all header fields by separate each line of headerSplit by ':'
		iterate each line of Header and create a map of < string, string >
		corresponding to < HeaderFileds, HeaderFileds value >
*/
void	ParseRequest::m_formated_HeaderFields(const std::vector<std::string> & headerSplit)
{
	size_t				found;
	std::string			key;
	std::string			value;
	std::string			line;
	std::vector<std::string>::const_iterator	it = headerSplit.begin();

	for (; it != headerSplit.end(); it++)
	{
		line = *it;
		found = line.find(":");
		if (found == std::string::npos)
			throw MessageErrorException(STATUS_BAD_REQUEST);
		key = std::string(&line[0], &line[found]);
		value = std::string(&line[found + 1], &line[line.size()]);
		m_headerFields[key] = value;
	}
}

/*
	Check host for symplify search for opening file
		if not in headerfield : 
		else
			if already in target : do nothing
			else cat host + target in requestLine
*/
void	ParseRequest::m_check_host_HeaderFields()
{
	// find Header Field "host"
	std::map<std::string, std::string>::iterator		found_host = m_headerFields.find("host");
	
	if (found_host == m_headerFields.end()) ///// FAUT IL OBLIGATOIREMENT LE HOST ? normalement oui avec http1.1
		throw MessageErrorException(400);

	// get header field value of "host"
	std::string		host = (*found_host).second;
	if (host.empty())
		throw MessageErrorException(400);

	// check if host is already in target
	if ( m_requestLine.target.find(host) == std::string::npos)
		m_requestLine.target = host + m_requestLine.target;
}


} // end namespace
