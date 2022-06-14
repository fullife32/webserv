/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/14 12:28:35 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

# include "MessageHTTP.hpp"
# include "Server.hpp"
# include <glob.h>

class ResponseHTTP : public MessageMethods, public HeaderFields,  public ContentTypes, public ErrorMap
{
	/*
		protected variables herited from MessageMethods: list of all Methods
			static	std::map <std::string, int>		m_methods;

		protected variables herited from HeaderFields: list of all Errors
			static std::map<int, std::string>	m_errors;

		protected variables herited from HeaderFields: map of HeaderFields
			std::map<std::string, std::string>	m_headerFields;

		protected variables herited from contentType: map of ContentType
			std::map< std::string, std::string> m_listContentType;
	*/
	private:

		StatusLine				m_statusLine;
		const ServerConf *		m_server;
		int						m_method;
		std::stringstream		m_header;
		std::ifstream			m_body;
		FILE *					m_body_CGI;
		size_t					m_body_CGI_size;
		size_t					m_length;
		URL						m_url;
		bool					m_isAutoindex;
		bool					m_is_redirection;




	public:

	/* constructor ------------------------------------------------ */
		ResponseHTTP(const ServerConf * server);
		ResponseHTTP(const ResponseHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ResponseHTTP();

	/* functions    ------------------------------------------------ */
		void		clear();
		size_t		size() ;

	/* get		    ------------------------------------------------ */
		bool		need_to_read() {
			if (m_body.good())
				return true;
			std::cout << false << std::endl;
			return false;
		}

		size_t		getNextChunk(char * buffer);
		FILE *		getBodyForCGI() const ;
		const URL & get_url() const ;
		std::string get_queryString() const ;
		std::string get_pathInfo() const ;
		int			get_method() const ;
		std::string get_serverName() const ;
		std::string get_fileName() const ;
		std::string get_path() const ;
		std::string get_port() const ;
		std::string	get_formatedPath() const;

	/* set		    ------------------------------------------------ */
		void		setRequestMethod(int method);
		void		setContentLength(size_t size);
		void		setContentType(std::string const & extension);
		void		setServer(ServerConf & server);
	
	/* build	    ------------------------------------------------ */
		void		buildError(int StatusCode, const std::string & ReasonPhrase, const URL & url);
		void		buildResponse(const RequestHTTP & request);

	private:

		void	m_set_minimalHeaderFields();
		void	m_setOpenFileBodySize();
		void	m_setCGIBodySize();


	/* formated Response   ------------------------------------------ */
		void	m_formated_Response();
		void	m_formated_StatusLine();
		void	m_formated_HeaderFields();
		void	m_formated_Autoindex( std::string &path );
		void	m_formated_CGI_Response(const RequestHTTP & request);

		void	m_formated_Error(const URL & url);
		void	m_formated_ErrorBody(std::stringstream & body);

		void	m_build_autoIndex(std::string location);

		void	m_openFile_CGI();
		void	m_openFile_Body(const std::string & location);
		bool	m_openFile_Error(const std::string & location);


	/* Methods    ------------------------------------------------ */
		void	m_parseMethod(const RequestHTTP & request);
		void	m_method_GET(const RequestHTTP & request);
		void	m_method_POST(const RequestHTTP & request);
		void	m_method_DELETE(const RequestHTTP & request);

		std::string	m_foundLocation();
		void		m_checkBodySize(size_t request_bodySize, size_t ContentLenght);

	// DEBUG
	public:
		virtual void	debug_print()
		{
			std::cout << " RESPONSE DEBUG PRINT ************************************" << std::endl;
			std::cout << "REQUESTLINE: " <<  std::endl;
			std::cout << "	status code: " << m_statusLine.statusCode << " " << m_statusLine.reasonPhrase << std::endl;
			std::cout << "	version: " << m_statusLine.version.name <<  m_statusLine.version.major_version << "." <<  m_statusLine.version.minor_version << std::endl;
			
			// std::cout << m_header.str() << std::endl;	
			
			// if (m_body.is_open())
			// {
			// 	m_body.seekg (0, m_body.beg);

			// 		char buf[1000] = {0};
			// 	while (!m_body.eof()) {
			// 		 m_body.getline(buf, 999);
			// 		std::cout << buf << std::endl;
			// 	}
			// 	m_body.seekg (0, m_body.beg);
			// }

		}


}; // end class RequestHTTP

#endif
