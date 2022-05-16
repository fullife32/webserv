/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_testRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 21:14:01 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/16 22:23:17 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

#define BUFFER_SIZE	32
#define DEBUG_WEBSERV

int main(int ac, char **argv)
{

	char	buf[BUFFER_SIZE];
	int 	fd;
	int		size_read;
	WS::ParseRequest	request;
	

	if (ac != 2)
		return (0);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		return (-1);
	}	
	
	do
	{
		size_read = read(fd, &buf, BUFFER_SIZE - 1);
		if (size_read != -1)
			request.append(buf);
		memset(buf, '\0', BUFFER_SIZE);
	} while (size_read > 0);
	

	/// TESTING SplitString

	std::vector<std::string> split = splitString(request.m_data, "\n\n");

	int i = 0;
	for (std::vector<std::string>::iterator it = split.begin(); it != split.end(); i++, it++)
		std::cout << i << " " <<  *it << std::endl;


	/// TESTING ParseRequest::m_separateHeaderBody()

	request.m_separateHeaderBody();

	std::cout << "HEADER : " << std::endl << request.m_header << std::endl;
	std::cout << "BODY : " << request.m_body.size() << std::endl << request.m_body << std::endl;
	

	// test formated_RequestLine
	WS::RequestLine					requestline;
	std::vector<std::string>	split_first_line;
	
	split = splitString(request.m_header, NEWLINE);

	for (std::vector<std::string>::iterator it = split.begin(); it != split.end(); i++, it++)
		std::cout << i << " " <<  *it << std::endl;

	split_first_line = splitString(split[0], " ");

for (std::vector<std::string>::iterator it = split_first_line.begin(); it != split_first_line.end(); i++, it++)
		std::cout << i << " " <<  *it << std::endl;
}
