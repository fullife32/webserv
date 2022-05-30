/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_testRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 21:14:01 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/30 09:58:49 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "MessageHTTP.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>

#define BUFFER_SIZE	2
#define DEBUG_WEBSERV




void	test_stream()
{
	std::string str;
	char buff[33];
	std::ifstream	fs("Error404.html", std::ifstream::in);
	// std::stringstream	ss;

	// fs >> ss;

	// str = ss.str();
	// std::cout << str << std::endl;
	// fs.readsome(buff, 31);
	// std::cout << buff << std::endl;


	// std::string str2(fs.beg(), fs.end());

  	// fs.seekg (0, fs.end);
    // int length = fs.tellg();
    // fs.seekg (0, fs.beg);

	// char *buff = new char[lenght];

	// fs.get(ss.str(), 370);
	
	// std::cout << buff << length;


		// fs.readsome()
		// fs.get(buff, 5, '\0');
		// std::cout << buff;

	std::stringstream		ss;
	ss << "HTML/1.1 200 OK";

	
	// std::ifstream	fs("Error404.html", std::ifstream::in);

	int bufferSize = 32;
	memset(buff, 0, bufferSize + 1);
	if (ss.read(buff,bufferSize))
		std::cout << buff << strlen(buff) << std::endl;
	else
		std::cout << buff << "|" << strlen(buff) << std::endl;
	
	// while (memset(buff, 0, 4) && ss.read(buff, 2))
	// 	std::cout << buff  << " | " << strlen(buff) << std::endl;
	// while (memset(buff, 0, 4) && fs.read(buff, 3))
	// 	std::cout << buff << " | " << strlen(buff) << std::endl;

}


int main(int ac, char **argv)
{


	char	buf[BUFFER_SIZE];
	int 	fd;
	int		size_read;
	WS::RequestHTTP		request;
	WS::ResponseHTTP	response;
	

	// test_stream();
	// return 0;

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

	// std::cout << response.getNextChunk(response.size()) << std::endl;

	// write like send()

	const char *buffer = response.getNextChunk(31);
	while (buffer != NULL)
	{
		std::cout << buffer;
		buffer = response.getNextChunk(31);
	}
	
	// for (size_t bufferSize = 0; bufferSize < response.size(); bufferSize += 13)
	// {
	// 	write(1, (response.getNextChunk(bufferSize)), response.getNextChunkSize(bufferSize));
	// }

}
