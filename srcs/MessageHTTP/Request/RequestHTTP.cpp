/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:04:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/12 13:58:28 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "MessageHTTP.hpp"

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

RequestHTTP::RequestHTTP(const ServerConf * server)
: ParseRequest(server)
{}


RequestHTTP::RequestHTTP(const RequestHTTP & copy)
: ParseRequest(copy)
{}

RequestHTTP::~RequestHTTP()
{}


/* -------------------------------------------------------------------------- */
// get

int		RequestHTTP::getMethod() const
{
	std::map <std::string, int>::const_iterator	found = m_methods.find(m_requestLine.method);
	if (found == m_methods.end())
		throw MessageErrorException(STATUS_BAD_REQUEST);
	return (*found).second;
}


URL		RequestHTTP::getUrl() const
{
	return (m_requestLine.url);
}

size_t	RequestHTTP::getBodySize() const
{
	return m_body_size;
}

FILE *		RequestHTTP::getBodyForCGI() const
{
	if (m_body != NULL)
		rewind(m_body);
	return m_body;
}

/* -------------------------------------------------------------------------- */

bool	RequestHTTP::hasQueryString() const
{
	return !(m_requestLine.url.query.empty());
}

bool	RequestHTTP::hasBody() const 
{
	return !(m_body_size == 0);
}
