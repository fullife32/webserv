#include "IMessageHTTP.hpp"
#include <iostream>
#include <cstdlib>

namespace WS 
{

	HTTPversion::HTTPversion()
	{}

	HTTPversion::HTTPversion(std::string version)
	{
		size_t	found_slash = version.find("/");
		size_t	found_point = version.find(".");

		if (found_slash == std::string::npos || found_point == std::string::npos)
			std::cerr << "THOW EXEPTION 400 BAD REQUEST" << std::endl;

		name = std::string(&version[0], &version[found_slash]);
		major_version = atoi(version.data() + found_slash + 1);
		minor_version = atoi(version.data() + found_point + 1);
	}

	HTTPversion::~HTTPversion()
	{}


} // end namespace
