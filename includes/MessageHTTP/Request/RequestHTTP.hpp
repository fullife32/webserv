/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 11:59:14 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include "AMessageHTTP.hpp"
#include <iostream>
#include "Utils.hpp"

namespace WS {




class RequestHTTP : public AMessageHTTP
{

	private:
		RequestLine							m_startLine;

	public:

		// constructor
		RequestHTTP();
		RequestHTTP(const RequestHTTP & copy);

		// destructor
		virtual ~RequestHTTP();

		// operator
		RequestHTTP &	operator=(const RequestHTTP & other);

		// functions
		RequestMethod	getMethod() const;


		// set
		void			setRequestLine(const RequestLine & requestLine);
		



		// debug
		virtual void	debug_print_startline()
		{
			HTTPversion	*v = &m_startLine.version;

			std::cout << "HTTPversion = " << v->name << " " << v->major_version << "." << v->minor_version << std::endl;
			std::cout << "URL = " << m_startLine.target << std::endl;
			std::cout << "method = " << m_startLine.method << std::endl;
		}

}; // end class RequestHTTP


} // end namespace

#endif
