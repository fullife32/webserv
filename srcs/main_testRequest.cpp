/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_testRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 21:14:01 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/23 15:31:12 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "ParseRequest.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE	2
#define DEBUG_WEBSERV

int main(int ac, char **argv)
{


	char	buf[BUFFER_SIZE];
	int 	fd;
	int		size_read;
	WS::ParseRequest	parserRequest;
	WS::RequestHTTP		request;
	WS::ResponseHTTP	response;
	
	// char *	t = "HTTP 1.1 coucou wesh coco\n";

	// for (size_t i = 0; i < strlen(t); i+= BUFFER_SIZE)
	// {
	// 	if (i + BUFFER_SIZE > strlen(t))
	// 		write(1, &t[i], strlen(t) - i);
	// 	else
	// 		write(1, &t[i], BUFFER_SIZE);
	// 	std::cout << std::endl;
		
	// }
	// return (0);


	if (ac != 2)
		return (0);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		// std::cerr << std::strerror(errno) << std::endl;
		return (-1);
	}	
	
	do
	{
		size_read = read(fd, &buf, BUFFER_SIZE - 1);
		if (size_read != -1)
			parserRequest.append(buf);
		memset(buf, '\0', BUFFER_SIZE);
	} while (size_read > 0);
	
	try
	{
		request.buildRequest(parserRequest);
		response.buildResponse(request);
	}
	catch (WS::MessageErrorException & e)
	{
		std::cerr << "error = " << e.getError() << " " << e.getMappedError() << std::endl;
	}

	for (size_t bufferSize = 0; bufferSize < response.size(); bufferSize += 3)
	{
		std::cout << response.getNextSend(3) << std::endl;
	}

}
