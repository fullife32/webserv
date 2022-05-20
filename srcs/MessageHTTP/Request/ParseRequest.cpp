/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:48:48 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/20 12:02:37 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"
#include "AMessageStruct.hpp"
#include "Utils.hpp"
#include <iostream>

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
		throw MessageErrorException(400);
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


/*
** After completely receive request : formated the requestHTTP
**	separate body and header
** formated the requestHeader in a requestLine
*/

RequestHTTP 	ParseRequest::getFormated_RequestHTTP()
{
	RequestHTTP					request;
	std::vector<std::string>	split;

	m_separateHeaderBody();

	split = splitString(m_header, NEWLINE);
	if (split.size() == 0)
		throw MessageErrorException(400);

	request.setRequestLine(m_formated_RequestLine(split[0]));
	split.erase(split.begin());
	request.setHeaderFields(m_formated_HeaderFields(split));
	request.setBody(m_body);
	
	return request;
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
		throw MessageErrorException(400);
	m_header = std::string(&m_data[0], &m_data[separation]);
	m_body =   std::string(&m_data[separation + 2], &m_data[m_data.size()]);
}

/*
** get first line of Header
		formated the first line of Request Header :
		Method SP URL SP HTTPversion

		for now HTTPversion is facultatif
		throw exception (400 Bad Request)
*/

RequestLine 		ParseRequest::m_formated_RequestLine(const std::string & startline)
{
	RequestLine					requestline;
	std::vector<std::string>	split;
	
	// voir ISSUE: rendre obligatoire ou non le HTTP
	split = splitString(startline, " ");
	if (split.size() < 2 || split.size() > 3)
		throw MessageErrorException(400);

	requestline.method = split[0];
	requestline.target = split[1];
	if (split.size() == 3)
		requestline.version.formatedVersion(split[2]);
	return (requestline);
}

/*
**	get all header fields by separate each line of headerSplit by ':'
		iterate each line of Header and create a map of < string, string >
		corresponding to < HeaderFileds, HeaderFileds value >
*/

std::map<std::string, std::string>	ParseRequest::m_formated_HeaderFields(const std::vector<std::string> & headerSplit)
{
	size_t				found;
	std::string			key;
	std::string			value;
	std::string			line;
	std::map<std::string, std::string>			map;
	std::vector<std::string>::const_iterator	it = headerSplit.begin();

	for (; it != headerSplit.end(); it++)
	{
		line = *it;
		found = line.find(":");
		if (found == std::string::npos)
			throw MessageErrorException(400);
		key = std::string(&line[0], &line[found]);
		value = std::string(&line[found + 1], &line[line.size()]);
		map[key] = value;
	}
	return map;
}

} // end namespace
