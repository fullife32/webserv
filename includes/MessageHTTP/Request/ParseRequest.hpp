/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:30:05 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/15 16:32:50 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "utils.hpp"
# include "MessageStruct.hpp"
# include "ErrorMessage.hpp"
# include "ServerConf.hpp"

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
		
	public:

	/* constructor ------------------------------------------------ */
		ParseRequest(const ServerConf * server);
		ParseRequest(const ParseRequest & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ParseRequest();

	/* public    ------------------------------------------------ */
		void	append(const char *buffer, size_t size);
		bool	empty() const ;
		void	clear() ;
		bool	is_end();


	private:
		void	m_append_body(const std::string & buffer);
		void	m_save_body(const char * buffer, size_t begin, size_t size);


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
		size_t	found_header_end(const char * buff, size_t size) const ;

}; // end class

#endif
