/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/17 16:31:28 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include "IMessageHTTP.hpp"
#include <iostream>
#include "utils.hpp"

namespace WS {

enum RequestMethod
{
	GET,
	POST,
	DELETE,
	PUT,	/////
	HEAD,
	CONNECT,
	OPTIONS,
	TRACE
};


class RequestHTTP : public IMessageHTTP
{

	private:
		RequestLine							m_start_line;

	public:

		// constructor
		RequestHTTP();
		RequestHTTP(const RequestHTTP & copy);

		// destructor
		virtual ~RequestHTTP();

		// operator
		RequestHTTP &	operator=(const RequestHTTP & other);

		// functions

		// set
		void			setRequestLine(const RequestLine & requestLine);
		void			setBody(const std::string & body);
		void			setHeaderFields(const std::map<std::string, std::string> & headerFields);




		// debug
		virtual void	debug_print_startline()
		{
			HTTPversion	*v = &m_start_line.version;

			std::cout << "HTTPversion = " << v->name << " " << v->major_version << "." << v->minor_version << std::endl;
			std::cout << "URL = " << m_start_line.target << std::endl;
			std::cout << "method = " << m_start_line.method << std::endl;
		}

}; // end class RequestHTTP


} // end namespace

#endif
