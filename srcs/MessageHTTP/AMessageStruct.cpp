/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessageStruct.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:37:45 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 15:32:56 by lvirgini         ###   ########.fr       */
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

		
	bool	HTTPversion::formatedVersion(const std::string & version)
	{

		// voir si obligatoire ou pas
		if (version.empty() == true)
			return false;
		size_t	found_slash = version.find("/");
		size_t	found_point = version.find(".");

		if (found_slash == std::string::npos || found_point == std::string::npos)
		{
			std::cerr << "THOW EXEPTION 400 BAD REQUEST" << std::endl;
			return false;
		}	
		name = std::string(&version[0], &version[found_slash]);
		major_version = atoi(version.data() + found_slash + 1);
		minor_version = atoi(version.data() + found_point + 1);
		return true;
	}

} // end namespace
