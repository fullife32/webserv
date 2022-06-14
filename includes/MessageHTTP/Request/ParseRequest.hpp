/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:30:05 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/14 20:04:15 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "utils.hpp"
# include "MessageStruct.hpp"
# include "ErrorMessage.hpp"
# include "ServerConf.hpp"
# include "Print.hpp" // TODO DEBUG

class ParseRequest : public HeaderFields
{
	protected:
		RequestLine			m_requestLine;
		FILE *				m_body;				// body for CGI post
		size_t				m_body_size;		// size of body request

	private:
		const ServerConf *	m_server;
		std::string			m_data;				// raw data from recv() client
		size_t				m_header_size;		// size of header request
		size_t				m_max_body_size; 	// max body size of location
		bool				m_is_post_method;
		bool				m_has_complete_header;
		bool				m_has_complete_startLine;
		
		// TODO delete ?
		bool				m_boundary_firstpart;
		bool				m_boundary_lastpart;
		std::string			m_boundary;  // TODO constructor et clear

	public:

	/* constructor ------------------------------------------------ */
		ParseRequest(const ServerConf * server);
		ParseRequest(const ParseRequest & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ParseRequest();

	/* public    ------------------------------------------------ */
		void	append(const std::string & buffer,char *buff, size_t size);
		bool	empty() const ;
		void	clear() ;

	private:
		void	m_append_body(const std::string & buffer);

		// TODO DELETE ?
		void	m_append_body_in_boundary();
		void	m_parse_boundary_firstline(std::string line);
		void	m_check_multipart_body();


	/* parsing    ------------------------------------------------ */
		bool	m_parse_header();
		void 	m_parse_RequestLine(const std::string & startline);
		void 	m_parse_headerFields(const std::string & line);
		void	m_parse_url(std::string url);
		void	m_parse_url_fragment(std::string & url);
		void	m_parse_url_query_string(std::string & url);
		void	m_parse_url_filename(std::string & url);
		void	m_parse_url_port(std::string & url);
		void	m_prepare_POST_body() ;




		void	m_check_max_header_size() const ;
		void	m_check_max_body_size() const ;
		void	m_check_host_HeaderFields();
		
	// TODO debug
	public:
		void			debug_print()
		{
			std::cout << "PARSE REQUEST DEBUG PRINT *******************************" << std::endl;
			std::cout << "REQUESTLINE: " <<  std::endl;
			std::cout << "	method: " << m_requestLine.method << std::endl;
			std::cout << "	version: " << m_requestLine.version.name <<  m_requestLine.version.major_version << "." <<  m_requestLine.version.minor_version << std::endl;
			std::cout << "	servername: |" << m_requestLine.url.serverName << "|" << std::endl;
			std::cout << "	port: |" << m_requestLine.url.port << "|" << std::endl;
			std::cout << "	path: " << m_requestLine.url.path << std::endl;
			std::cout << "	filename: " << m_requestLine.url.filename << std::endl;
			std::cout << "	extension: " << m_requestLine.url.fileExtension << std::endl;
			std::cout << "	query: " << m_requestLine.url.query << std::endl;
			std::cout << "	pathInfo: "<< m_requestLine.url.pathInfo << std::endl;
			std::cout << "	fragment: " << m_requestLine.url.fragment << std::endl;
			std::cout << "Boundary =" << m_boundary << std::endl;

		std::map<std::string, std::string>::iterator	it;

	for (it = m_headerFields.begin(); it != m_headerFields.end(); it++)
		std::cout << "key=" << it->first << " value=" << it->second << std::endl;
		}

}; // end class

#endif
