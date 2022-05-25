/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:04:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/25 12:15:38 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHTTP.hpp"

namespace WS
{

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

	RequestHTTP::RequestHTTP()
		: AMessageHTTP(),
		m_startLine(),
		m_parseRequest()
	{}


	RequestHTTP::RequestHTTP(const RequestHTTP & copy)
		: AMessageHTTP(copy),
		m_startLine(copy.m_startLine),
		m_parseRequest(copy.m_parseRequest)
	{	}

	RequestHTTP::~RequestHTTP()
	{}

/* -------------------------------------------------------------------------- */


	void	RequestHTTP::append(const std::string & buffer)
	{
		m_parseRequest.append(buffer);
	}

	void	RequestHTTP::buildRequest()
	{
		m_startLine = m_parseRequest.getRequestLine();
		m_header_fields = m_parseRequest.getHeaderFields();
		m_body = m_parseRequest.getBody();
	}

/* -------------------------------------------------------------------------- */
// set:

	void	RequestHTTP::setRequestLine(const RequestLine & requestline)
	{
		m_startLine = requestline;
	}

/* -------------------------------------------------------------------------- */
// get

	int		RequestHTTP::getMethod() const
	{
		std::map <std::string, int>::const_iterator	found = m_methods.find(m_startLine.method);
		if (found == m_methods.end())
			throw MessageErrorException(STATUS_BAD_REQUEST);
		return (*found).second;
	}

} // end namespace
