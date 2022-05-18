/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:04:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/18 15:40:20 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHTTP.hpp"

namespace WS
{

	RequestHTTP::RequestHTTP()
	{}


	RequestHTTP::RequestHTTP(const RequestHTTP & copy)
	: IMessageHTTP(copy),
	m_start_line(copy.m_start_line)
	{}

	RequestHTTP::~RequestHTTP()
	{}









// set:

	void	RequestHTTP::setRequestLine(const RequestLine & requestline)
	{
		m_start_line = requestline;
	}



} // end namespace
