/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/25 11:08:45 by lvirgini         ###   ########.fr       */
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

		std::stringstream		m_dataResponse;
		std::string				m_data;
		StatusLine				m_startLine;
		int						m_method;
		size_t					m_chunk;

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

		void		clear();
		size_t		size() const ;

		void			buildResponse(const RequestHTTP & request);
		const char *	getNextChunk(size_t BufferSize);
		size_t			getNextChunkSize(size_t BufferSize) const;



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
