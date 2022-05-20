/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_testRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 21:14:01 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/20 12:17:59 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <ctime>

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
	
	try
	{
		WS::RequestHTTP		final_request = request.getFormated_RequestHTTP();
		final_request.debug_print_Message();
	}
	catch (WS::MessageErrorException & e)
	{
		std::cerr << "error = " << e.getError() << " " << e.getMappedError() << std::endl;
	}

}
