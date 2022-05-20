/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageStruct.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:37:45 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/20 12:00:00 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessageStruct.hpp"
#include <iostream>
#include <cstdlib>

namespace WS 
{
	HTTPversion::HTTPversion()
	: name(), major_version(), minor_version()
	{}

	HTTPversion::~HTTPversion()
	{}

		
	void	HTTPversion::formatedVersion(const std::string & version)
	{
		if (version.empty() == true)
			return ;
			
		size_t	found_slash = version.find("/");
		size_t	found_point = version.find(".");

		if (found_slash == std::string::npos || found_point == std::string::npos)
			throw	WS::MessageErrorException(400);
		name = std::string(&version[0], &version[found_slash]);
		major_version = atoi(version.data() + found_slash + 1);
		minor_version = atoi(version.data() + found_point + 1);
	}

} // end namespace
