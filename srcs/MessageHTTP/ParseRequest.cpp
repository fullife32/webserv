/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:48:48 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/16 19:54:38 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"
#include "usefull.hpp"
#include <iostream>

namespace WS {

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


std::string &	ParseRequest::append(const std::string & str)
{
	return m_data.append(str);
}


RequestHTTP 	ParseRequest::getFormated_RequestHTTP()
{
	RequestHTTP		request;

	m_separateHeaderBody();
	request.setRequestLine(m_formated_RequestLine());
	request.setBody(m_body);
	
	return request;
}


void			ParseRequest::m_separateHeaderBody()
{
	size_t		separation = m_data.find(HEADER_SEPARATOR);
	
	if (separation == std::string::npos)
		std::cerr << "THROW EXCEPTION 400 BAD REQUEST" << std::endl;		///// throw
	m_header = std::string(m_data.at(0), m_data.at(separation));
	m_body =   std::string(m_data.at(separation + HEADER_SEPARATOR_SIZE), m_data.at(m_data.size()));
}





RequestLine 		ParseRequest::m_formated_RequestLine()
{
	RequestLine					requestline;
	std::vector<std::string>	split;
	
	split = splitString(m_header, NEWLINE);


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
