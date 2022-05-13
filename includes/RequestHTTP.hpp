/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/13 15:02:39 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

#include "IMessageHTTP.hpp"

namespace WS {

class RequestHTTP : public IMessageHTTP
{

	private:
		RequestLine			start_line;

	public:

		// constructor
		RequestHTTP();
		RequestHTTP(const RequestHTTP & copy);

		// destructor
		virtual ~RequestHTTP();

		// operator
		RequestHTTP &	operator=(const RequestHTTP & other);

		// functions







}; // end class RequestHTTP


} // end namespace

#endif
