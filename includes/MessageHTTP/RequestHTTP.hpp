/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/16 19:08:52 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include "IMessageHTTP.hpp"

namespace WS {

enum RequestMethod
{
	GET,
	POST,
	DELETE,
	PUT,
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




}; // end class RequestHTTP


} // end namespace

#endif
