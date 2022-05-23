/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:04:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/23 15:27:45 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHTTP.hpp"

namespace WS
{

	RequestHTTP::RequestHTTP()
	: AMessageHTTP(),
	m_startLine()
	{}


	RequestHTTP::RequestHTTP(const RequestHTTP & copy)
		: AMessageHTTP(copy),
		m_startLine(copy.m_startLine)
	{
		m_methods.begin(); ///
	}

	RequestHTTP::~RequestHTTP()
	{}



	void	RequestHTTP::buildRequest(ParseRequest & parser)
	{
		m_startLine = parser.getRequestLine();
		m_header_fields = parser.getHeaderFields();
		m_body = parser.getBody();
	}





// set:

	void	RequestHTTP::setRequestLine(const RequestLine & requestline)
	{
		m_startLine = requestline;
	}


// get

	int		RequestHTTP::getMethod() const
	{
		std::map <std::string, int>::const_iterator	found = m_methods.find(m_startLine.method);
		if (found == m_methods.end())
			throw MessageErrorException(STATUS_BAD_REQUEST);
		return (*found).second;
	}




} // end namespace
