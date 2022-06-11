/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_testRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 21:14:01 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/11 12:11:19 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "MessageHTTP.hpp"
#include "ParseRequest.hpp"
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

#define DEBUG_WEBSERV

#define BUFFER_SIZE	32


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


void test_fstream()
{

	// testing multi open same file with an insert for one and read for the other
	// std::fstream	fst("srcs");
	// std::fstream	fst2("testParseRequest");
	// std::fstream	fst3("testParseRequest");

	// std::cout << "is open " << fst.is_open() << std::endl;
	// std::cout << "is open " << fst2.is_open() << std::endl;
	// std::cout << "is open " << fst3.is_open() << std::endl;

	// std::cout << "good : " << fst.good() << std::endl;
	// std::cout << "good : " << fst2.good() << std::endl;
	// std::cout << "good : " << fst3.good() << std::endl;


	// fst3.seekg(0, fst3.end);

	// char	buf[BUFFER_SIZE + 1];

	// memset(buf, 0, BUFFER_SIZE);

	// while (fst2.read(buf, BUFFER_SIZE))
	// {
	// 	fst3 << "coucou";
	// 	std::cout << buf;
	// 	memset(buf, 0, BUFFER_SIZE);
	// }
	// return (0);

}


void test_request_response(int ac, char **argv)
{

	char	buf[MESSAGE_BUFFER_SIZE + 1];
	int 	fd;
	int		size_read;
	RequestHTTP		request;
	ResponseHTTP	response(NULL);
	
	memset(buf, '\0', MESSAGE_BUFFER_SIZE);
	

	if (ac != 2)
		return ;
// 	// open file for test
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		return ;
	}	
	

	// get buffer like recv
	do
	{
		size_read = read(fd, &buf, MESSAGE_BUFFER_SIZE);
		if (size_read != -1)
			request.append(buf);
		memset(buf, '\0', MESSAGE_BUFFER_SIZE);
	} while (size_read > 0);
	

	// end recv : client try to build request and response
	try
	{
		// request.buildRequest();
		response.buildResponse(request);
	}
	catch (MessageErrorException & e)
	{
		response.buildError(e.getError(), e.getMappedError(), e.getUrl());
	}

	// std::cout << response.getNextChunk(response.size()) << std::endl;

	// write like send()

	// const char *buffer = response.getNextChunk();
	// while (buffer != NULL)
	// {
	// 	std::cout << buffer;
	// 	buffer = response.getNextChunk();
	// }
	
// 	// for (size_t bufferSize = 0; bufferSize < response.size(); bufferSize += 13)
// 	// {
// 	// 	write(1, (response.getNextChunk(bufferSize)), response.getNextChunkSize(bufferSize));
// 	// }
}
int main(int ac, char **argv)
{

	test_request_response(ac, argv);
	// int fd = open("test_config/simple/simple.html", O_RDONLY);

	// fd.read
	// std::fstream	fs;
	// fs.open(fd)
	// char *buf[1000];

	// fs.read(buf, 1000 - 1);
	// std::cout << buf << std::endl

	// ParseRequest	r;


	// r.m_formated_Url("//////");
	// r.m_formated_Url("///");
	// r.m_formated_Url("/");
	// r.m_formated_Url("/path/.index.html.rc");
	// r.m_formated_Url("/path/with/most/directory/and/there/is/not/a/file");
	// r.m_formated_Url("/path/with/most/directory/file.txt?queryString=jeNeSaisPas#fragmentUtils???");
	// r.debug_print();

	return (0);
}
