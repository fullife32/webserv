/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/18 15:35:44 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include "IMessageHTTP.hpp"
#include <iostream>
#include "utils.hpp"

namespace WS {




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
		RequestMethod	getMethod() const;


		// set
		void			setRequestLine(const RequestLine & requestLine);
		



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
