/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageStruct.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:37:45 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/30 10:23:45 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"
#include <iostream>
#include <cstdlib>

namespace WS 
{

	/* HTTPversion  ------------------------------------------------ */

	HTTPversion::HTTPversion()
	: name(), major_version(), minor_version()
	{}

	HTTPversion::~HTTPversion()
	{}

	/*
		HTTP version = HTTP-name "/" major_version "." minor_version
	*/

	void	HTTPversion::formatedVersion(const std::string & version)
	{
		if (version.empty() == true)
			return ;
			
		size_t	found_slash = version.find("/");
		size_t	found_point = version.find(".");

		if (found_slash == std::string::npos || found_point == std::string::npos)
			throw	MessageErrorException(STATUS_BAD_REQUEST);
		name = std::string(&version[0], &version[found_slash]);
		major_version = atoi(version.data() + found_slash + 1);
		minor_version = atoi(version.data() + found_point + 1);
		if (isSupportedVersion() == false)
			throw	MessageErrorException(STATUS_HTTPVERSION_NOT_SUPPORTED);
	}

	bool	HTTPversion::isSupportedVersion()
	{
		if (major_version != 1 || minor_version != 1)
			return false;
		return true;
	}

	void	HTTPversion::clear()
	{
		name.clear();
		major_version = 0;
		minor_version = 0;
	}


	/* Start Line ------------------------------------------------ */

	void	RequestLine::clear()
	{
		version.clear();
		target.clear();
		method.clear();
	}


	void	StatusLine::clear()
	{
		version.clear();
		statusCode = 0;
		reasonPhrase.clear();
	}


	/* Message Methods static map  --------------------------------- */

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