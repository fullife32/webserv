/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/23 15:43:35 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include "ResponseHTTP.hpp"
#include "RequestHTTP.hpp"
#include <string>
#include <sstream>

namespace WS {

class ResponseHTTP : public AMessageHTTP
{
	/*
		protected variables herited from AMessageHTTP:

		StartLine								m_startLine;
		std::map <std::string, std::string> 	m_header_fields;
		std::string								m_body;
		static	std::map <std::string, int>		m_methods;
	*/

	private:

		StatusLine				m_startLine;
		int						m_method;
		std::stringstream		m_dataResponse;
		size_t					m_chunk;
		std::string::iterator	m_it_send;
		char *					ptr;

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
	
	
	/* functions    ------------------------------------------------ */

		void	clear();
		void	buildResponse(const RequestHTTP & request);
		char *	getNextChunk(size_t BufferSize);
		size_t	size() const ;


		// void	send(size_t	begin, size_t size);
	private:

		void	m_minimalHeaderFields();
		void	m_formatedResponse();

		void	m_parseMethod();
		void	m_method_GET();
		void	m_method_POST();
		void	m_method_DELETE();


	// DEBUG
	public:
		virtual void	debug_print_startline()
		{
			HTTPversion	*v = &m_startLine.version;

			std::cout << "HTTPversion = " << v->name << " " << v->major_version << "." << v->minor_version << std::endl;
		}



}; // end class RequestHTTP

} // end namespace

#endif
