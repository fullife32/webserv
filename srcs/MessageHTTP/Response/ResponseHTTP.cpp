/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 15:34:21 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHTTP.hpp"

namespace WS
{

	ResponseHTTP::ResponseHTTP()
	: AMessageHTTP(),
	m_startLine()
	{
		m_minimalHeaderFields();
	}


	ResponseHTTP::~ResponseHTTP()
	{}




	void	ResponseHTTP::m_minimalHeaderFields()
	{
		// m_header_fields[]
	}



} // end namespace


