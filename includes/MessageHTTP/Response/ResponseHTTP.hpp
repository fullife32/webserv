/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/04 11:49:53 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include "MessageHTTP.hpp"
# include "Server.hpp"

namespace WS {

class ResponseHTTP : public MessageMethods, public HeaderFields
{
	/*
		protected variables herited from MessageMethods: list of all Methods
			static	std::map <std::string, int>		m_methods;

		protected variables herited from HeaderFields: list of all Methods
			std::map<std::string, std::string>	m_headerFields;	
	*/
	private:

		// classCGI				m_cgi;
		StatusLine				m_requestLine;
		Server *				m_server;
		int						m_method;
		std::stringstream		m_header;
		std::fstream			m_body;
		size_t					m_length;
		char					*m_buffer;
		//TODO error page check if exist else create it
		//TODO test DELETE / 
		

	public:

	/* constructor ------------------------------------------------ */
		ResponseHTTP();
		ResponseHTTP(Server * server, char *m_buffer);
		ResponseHTTP(const ResponseHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ResponseHTTP();


	/* operator    ------------------------------------------------ */
		ResponseHTTP &	operator=(const ResponseHTTP & other);


	/* get		    ------------------------------------------------ */
		size_t	getNextChunk();

	/* set		    ------------------------------------------------ */
		void		setRequestMethod(int method);
		void		setContentLength(size_t size);
		void		setServer(Server & server);
	
	
	/* functions    ------------------------------------------------ */

		void		clear();
		size_t		size() ;


		void		buildError(int StatusCode, const std::string &  ReasonPhrase);
		void		buildResponse(const RequestHTTP & request);


	private:

		// set
		void	m_set_minimalHeaderFields();

	/* formated Response   ------------------------------------------ */
		void	m_formated_Response(const std::string & url);
		void	m_formated_StatusLine();
		void	m_formated_HeaderFields();
		void	m_formated_CGI_Response(const RequestHTTP & request);
		void	m_formated_Error();
		void	m_formated_ErrorBody(std::stringstream & body);
		void	m_openFile_Body(const std::string & url);

	/* Methods    ------------------------------------------------ */
		void	m_parseMethod(const RequestHTTP & request);
		void	m_method_GET(const RequestHTTP & request);
		void	m_method_POST(const RequestHTTP & request);
		void	m_method_DELETE();

		void	m_foundLocation();


	// DEBUG
	public:
		virtual void	debug_print_startline()
		{
			HTTPversion	*v = &m_requestLine.version;

			std::cout << "HTTPversion = " << v->name << " " << v->major_version << "." << v->minor_version << std::endl;
		}


}; // end class RequestHTTP

} // end namespace

#endif
