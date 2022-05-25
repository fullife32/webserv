/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_testRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 21:14:01 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/25 13:09:58 by lvirgini         ###   ########.fr       */
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
	WS::RequestHTTP		request;
	WS::ResponseHTTP	response;
	

	if (ac != 2)
		return (0);

	// open file for test
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		return (-1);
	}	
	

	// get buffer like recv
	do
	{
		size_read = read(fd, &buf, BUFFER_SIZE - 1);
		if (size_read != -1)
			request.append(buf);
		memset(buf, '\0', BUFFER_SIZE);
	} while (size_read > 0);
	

	// end recv : client try to build request and response
	try
	{
		request.buildRequest();
		response.buildResponse(request);
	}
	catch (WS::MessageErrorException & e)
	{
		response.buildError(e.getError(), e.getMappedError());
	}


	std::cout << response.getNextChunk(response.size()) << std::endl;

	// write like send()
	for (size_t bufferSize = 0; bufferSize < response.size(); bufferSize += 13)
	{
		write(1, (response.getNextChunk(bufferSize)), response.getNextChunkSize(bufferSize));
	}

}
