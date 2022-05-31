/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/31 11:59:16 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include "MessageHTTP.hpp"

namespace WS {

class ResponseHTTP : public MessageMethods
{
	/*
		protected variables herited from MessageMethods:

		static	std::map <std::string, int>		m_methods;
	*/

	private:

		std::map <std::string, std::string>	m_headerFields;
		StatusLine				m_requestLine;
		int						m_method;
		std::stringstream		m_header;
		std::fstream			m_body;
		size_t					m_length;
		char					*m_bufferToSend;

	public:

	/* constructor ------------------------------------------------ */
		ResponseHTTP();
		ResponseHTTP(const RequestHTTP & request);
		ResponseHTTP(const ResponseHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ResponseHTTP();


	/* operator    ------------------------------------------------ */
		ResponseHTTP &	operator=(const ResponseHTTP & other);


	/* set		    ------------------------------------------------ */
		void		setRequestMethod(int method);
		void		setContentLength(size_t size);
		void		setHeaderFields(const std::string & headerField, const std::string & value);
	
	
	/* functions    ------------------------------------------------ */

		void		clear();
		size_t		size() ;

		void			buildError(int StatusCode, const std::string &  ReasonPhrase);
		void			buildResponse(const RequestHTTP & request);
		const char *	getNextChunk(size_t BufferSize);


	private:

		void	m_minimalHeaderFields();
		void	m_formated_Response(const std::string & url);
		void	m_formated_StatusLine();
		void	m_formated_HeaderFields();
		void	m_formated_Error();
		void	m_formated_ErrorBody(std::stringstream & body);

		void	m_openFile_Body(const std::string & url);

		void	m_parseMethod();
		void	m_method_GET();
		void	m_method_POST();
		void	m_method_DELETE();


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
