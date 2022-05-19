/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 10:33:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 10:37:57 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessageHTTP.hpp"

namespace WS 
{

	void	AMessageHTTP::setBody(const std::string & body)
	{
		m_body = body;
	}

	void	AMessageHTTP::setHeaderFields(const std::map<std::string, std::string> & headerFields)
	{
		m_header_fields = headerFields;
	}

} // end namespace
