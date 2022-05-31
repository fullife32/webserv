/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 10:33:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/30 09:52:28 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
// #include <map>
// #include <string>

namespace WS 
{

	// /* constructor ------------------------------------------------ */

	// AMessageHTTP::AMessageHTTP()
	// 	: m_header_fields()
	// {}


	// AMessageHTTP::AMessageHTTP(const AMessageHTTP & copy)
	// 	: m_header_fields(copy.m_header_fields)
	// {}

	// /* destructor  ------------------------------------------------ */
	// AMessageHTTP::~AMessageHTTP()
	// {}


	// /* set		    ------------------------------------------------ */
	// // void	AMessageHTTP::setBody(const std::string & body)
	// // {
	// // 	m_body = body;
	// // }

	// void	AMessageHTTP::setHeaderFields(const std::map<std::string, std::string> & headerFields)
	// {
	// 	m_header_fields = headerFields;
	// }




std::map <std::string, int>		init_map_method()
{
	std::map<std::string, int>	methods;

	methods[METHOD_GET]		= GET;
	methods[METHOD_POST]	= POST;	
	methods[METHOD_DELETE]	= DELETE;
	methods[METHOD_HEAD]	= HEAD;
	methods[METHOD_PUT]		= PUT;
	methods[METHOD_CONNECT]	= CONNECT;
	methods[METHOD_OPTIONS]	= OPTIONS;
	methods[METHOD_TRACE]	= TRACE;

	return methods;
}


std::map <std::string, int>		MessageMethods::m_methods = init_map_method();

} // end namespace
