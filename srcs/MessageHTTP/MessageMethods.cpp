/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 10:33:50 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/07 16:39:53 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"

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
