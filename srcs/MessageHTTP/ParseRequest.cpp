/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:48:48 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/16 22:26:22 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"
#include "usefull.hpp"
#include <iostream>

namespace WS {

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

ParseRequest::ParseRequest()
{}


ParseRequest::ParseRequest(std::string	data)
	: m_data(data),
	m_header_begin(data.begin()),
	m_body_end(data.end())
{
	if (data.empty())
		std::cerr << "throw 400 BAD REQUEST" << std::endl; /// TRHOW ?
}


ParseRequest::~ParseRequest()
{}


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
	RequestHTTP		request;

	m_separateHeaderBody();
	request.setRequestLine(m_formated_RequestLine());
	request.setBody(m_body);
	
	return request;
}

/*
** Separate Header from Body
*/

void			ParseRequest::m_separateHeaderBody()
{
	size_t		separation = m_data.find(MESSAGE_END);
	
	if (separation == std::string::npos)
		std::cerr << "THROW EXCEPTION 400 BAD REQUEST" << std::endl;		///// throw si pas de \n\n la request n'est pas correcte
	m_header = std::string(&m_data[0], &m_data[separation]);
	m_body =   std::string(&m_data[separation + 2], &m_data[m_data.size()]);
}


RequestLine 		ParseRequest::m_formated_RequestLine()
{
	RequestLine					requestline;
	std::vector<std::string>	split;
	std::vector<std::string>	split_first_line;
	
	split = splitString(m_header, NEWLINE);
	if (split.size() == 0)
		std::cerr << "THROW EXCEPTION 400 BAD REQUEST" << std::endl;		///// throw si pas de \n\n la request n'est pas correcte
	split_first_line = splitString(split[0], " ");
	requestline.method = split[0];
	requestline.target = split[1];
	// requestline.version(split[2]);


	return requestline;
}




// void			ParseRequest::m_settingUpIterators()
// {
// 	m_header_end = std::string::iterator(&m_data[m_data.find("\r\n\r\n")]);
// 	m_body_begin = m_header_end + 1;
// }


// std::vector<std::string>		splitString(std::string::iterator begin, std::string::iterator end)
// {
// 	std::string					line;
// 	std::vector<std::string>	split;
// 	std::string::iterator		endline = begin;

// 	while (endline != end)
// 	{
// 		if (*endline)
// 	}
// }

} // end namespace
