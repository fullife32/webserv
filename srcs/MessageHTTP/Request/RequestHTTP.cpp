/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:04:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 11:34:06 by lvirgini         ###   ########.fr       */
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
	{}

	RequestHTTP::~RequestHTTP()
	{}









// set:

	void	RequestHTTP::setRequestLine(const RequestLine & requestline)
	{
		m_startLine = requestline;
	}




} // end namespace
