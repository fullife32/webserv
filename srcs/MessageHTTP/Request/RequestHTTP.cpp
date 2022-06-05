/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:04:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/05 17:08:48 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
namespace WS
{

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

	RequestHTTP::RequestHTTP()
		: m_requestLine(),
		m_parseRequest()
	{}


	RequestHTTP::RequestHTTP(const RequestHTTP & copy)
		: m_requestLine(copy.m_requestLine),
		m_parseRequest(copy.m_parseRequest)
	{
	}

	RequestHTTP::~RequestHTTP()
	{}

/* -------------------------------------------------------------------------- */


	void	RequestHTTP::append(const char * buffer)
	{
		m_parseRequest.append(buffer);
	}

	void	RequestHTTP::buildRequest()
	{
		m_parseRequest.m_prepareRequestBuilding();
		m_requestLine = m_parseRequest.getRequestLine();
		m_headerFields = m_parseRequest.getHeaderFields();
		m_body = m_parseRequest.getBody();
	}

/* -------------------------------------------------------------------------- */
// set:

	void	RequestHTTP::setRequestLine(const RequestLine & requestline)
	{
		m_requestLine = requestline;
	}

	void	RequestHTTP::setBody(const std::string & body)
	{
		m_body = body;
	}

/* -------------------------------------------------------------------------- */
// get

	int		RequestHTTP::getMethod() const
	{
		std::map <std::string, int>::const_iterator	found = m_methods.find(m_requestLine.method);
		if (found == m_methods.end())
		{
			
			// std::cout << "get method : " << std::endl;
			throw MessageErrorException(STATUS_BAD_REQUEST);
		}
		return (*found).second;
	}


	URL		RequestHTTP::getUrl() const
	{
		return (m_requestLine.url);
	}


/* -------------------------------------------------------------------------- */

	bool	RequestHTTP::hasQueryString() const
	{
		return !(m_requestLine.url.query.empty());
	}

	bool	RequestHTTP::hasBody() const 
	{
		// std::cout << "IN HAS BODY " << m_body << !(m_body.empty()) << std::endl;
		return !(m_body.empty());
	}
	
} // end namespace
