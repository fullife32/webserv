/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/08 13:55:34 by rotrojan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

# include "Server.hpp"
# include "MessageHTTP.hpp"

class ResponseHTTP : public MessageMethods, public HeaderFields, public ErrorMap
{
	/*
		protected variables herited from MessageMethods: list of all Methods
			static	std::map <std::string, int>		m_methods;

		protected variables herited from HeaderFields: list of all Errors
			static std::map<int, std::string>	m_errors;

		protected variables herited from HeaderFields: map of HeaderFields
			std::map<std::string, std::string>	m_headerFields;

		
	*/
	private:

		// classCGI				m_cgi;
		StatusLine				m_statusLine;
		const ServerConf *		m_server;
		int						m_method;
		std::stringstream		m_header;
		std::fstream			m_body;
		size_t					m_length;
		bool					m_isAutoindex;
		URL						m_url;
		//TODO test DELETE / 
		

	public:

	/* constructor ------------------------------------------------ */
		ResponseHTTP();
		ResponseHTTP(const ServerConf * server);
		ResponseHTTP(const ResponseHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ResponseHTTP();


	/* operator    ------------------------------------------------ */
		ResponseHTTP &	operator=(const ResponseHTTP & other);


	/* get		    ------------------------------------------------ */
		size_t		getNextChunk(char * buffer);
		const URL & get_url() const ;
		std::string get_queryString() const ;
		std::string get_pathInfo() const ;
		int			get_method() const ;
		std::string get_serverName() const ;


	/* set		    ------------------------------------------------ */
		void		setRequestMethod(int method);
		void		setContentLength(size_t size);
		void		setServer(ServerConf & server);
	
	
	/* functions    ------------------------------------------------ */

		void		clear();
		size_t		size() ;

		void		buildError(int StatusCode, const std::string & ReasonPhrase, const URL & url);
		void		buildResponse(const RequestHTTP & request);

	private:

		// set
		void	m_set_minimalHeaderFields();
		void	m_setOpenFileBodySize();

	/* formated Response   ------------------------------------------ */
		void	m_formated_Response();
		void	m_formated_StatusLine();
		void	m_formated_HeaderFields();
		void	m_formated_CGI_Response(const RequestHTTP & request);

		void	m_formated_Error(const URL & url);
		void	m_formated_ErrorBody(std::stringstream & body);

		void	m_build_autoIndex(std::string location);

		void	m_openFile_Body(const std::string & location);
		bool	m_openFile_Error(const std::string & location);


	/* Methods    ------------------------------------------------ */
		void	m_parseMethod(const RequestHTTP & request);
		void	m_method_GET(const RequestHTTP & request);
		void	m_method_POST(const RequestHTTP & request);
		void	m_method_DELETE(const URL & url);

		std::string	m_foundLocation();
		void	m_checkBodySize(size_t request_bodySize, size_t ContentLenght);

	// DEBUG
	public:
		virtual void	debug_print()
		{
			std::cout << " RESPONSE DEBUG PRINT ************************************" << std::endl;
			// std::string	S;
			// m_header >> S;
			// std::cout << "HEADER: " << S.data() << std::endl;
			// m_body >> S;
			// std::cout << "BODY: " << S.data() << std::endl;
			std::cout << "REQUESTLINE: " <<  std::endl;
			std::cout << "	status code: " << m_statusLine.statusCode << " " << m_statusLine.reasonPhrase << std::endl;
			std::cout << "	version: " << m_statusLine.version.name <<  m_statusLine.version.major_version << "." <<  m_statusLine.version.minor_version << std::endl;
			std::cout << std::endl;		
		}


}; // end class RequestHTTP

#endif
