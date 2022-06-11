/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:30:05 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/11 13:15:06 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "utils.hpp"
# include "MessageStruct.hpp"
# include "ErrorMessage.hpp"
# include "Print.hpp" // TODO DEBUG
# include <iostream> // TODO DEBUG

// std::string		extract_line(std::string & str, size_t	size);


class ParseRequest : public HeaderFields
{
	protected:
		RequestLine							m_requestLine;
		FILE *								m_body;				// body for CGI post
		size_t								m_body_size;		// size of body request
		size_t								m_max_body_size; 	// TODO get from server


	private:
		std::string							m_data;				// raw data from recv() client
		size_t								m_header_size;		// size of header request
		bool								m_is_post_method;
		bool								m_has_complete_header;
		bool								m_has_complete_startLine;

	public:

	/* constructor ------------------------------------------------ */
		ParseRequest();
		ParseRequest(const ParseRequest & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ParseRequest();


	/* public    ------------------------------------------------ */
		void		append(const std::string & str); 		// pour recuperer la requete entiere ( buffer)
		bool 		empty() const ;
		FILE *		getCGIbody(); // TODO
	
	

	private:
		void	m_prepare_POST_body() ;
		void	m_append_body(const std::string & str); 	// pour recuperer la requete entiere ( buffer)


	/* parsing    ------------------------------------------------ */

		bool	m_parse_header() ; // parse raw data and check if header is complete
		void 	m_parse_RequestLine(const std::string & startline);
		void 	m_parse_headerFields(const std::string & line);
		void	m_parse_url(std::string url);


		void	m_check_max_header_size() const ;
		void	m_check_max_body_size() const ;
		void	m_check_host_HeaderFields(const std::string & url); // TODO
		
		// bool	m_is_POST_method() const ; // MAYBE NOT NEED
		// bool	m_has_complete_header() const ; // MAYBE NOT NEED ?



	// void	ParseRequest::set_maxBodySize(size_t max) // TODO



	//debug

	public:
		void			debug_print()
		{
			std::cout << "PARSE REQUEST DEBUG PRINT *******************************" << std::endl;
			std::cout << "REQUESTLINE: " <<  std::endl;
			std::cout << "	method: " << m_requestLine.method << std::endl;
			std::cout << "	version: " << m_requestLine.version.name <<  m_requestLine.version.major_version << "." <<  m_requestLine.version.minor_version << std::endl;
			std::cout << "	servername: " << m_requestLine.url.serverName << std::endl;
			std::cout << "	path: " << m_requestLine.url.path << std::endl;
			std::cout << "	filename: " << m_requestLine.url.filename << std::endl;
			std::cout << "	extension: " << m_requestLine.url.fileExtension << std::endl;
			std::cout << "	query: " << m_requestLine.url.query << std::endl;
			std::cout << "	fragment: " << m_requestLine.url.fragment << std::endl;

		std::map<std::string, std::string>::iterator	it;

	for (it = m_headerFields.begin(); it != m_headerFields.end(); it++)
		std::cout << "key=" << it->first << " value=" << it->second << std::endl;
		}

}; // end class

#endif

