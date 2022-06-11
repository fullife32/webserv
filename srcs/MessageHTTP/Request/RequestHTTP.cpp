/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:04:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/11 13:13:43 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RequestHTTP.hpp"

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

RequestHTTP::RequestHTTP()
{}


RequestHTTP::RequestHTTP(const RequestHTTP & copy)
{
}

RequestHTTP::~RequestHTTP()
{}


/* -------------------------------------------------------------------------- */
// get

int		RequestHTTP::getMethod() const
{
	std::map <std::string, int>::const_iterator	found = m_methods.find(m_requestLine.method);
	if (found == m_methods.end())
	{
		
		std::cout << "get method :  |"<<  m_requestLine.method << "|" << std::endl;
		throw MessageErrorException(STATUS_BAD_REQUEST);
	}
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

int				get_body_for_CGI()
{
	// m_bodyCGI = open(m_path_body_for_CGI.str(), O_RDONLY);
	// if (m_bodyCGI == -1)
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
