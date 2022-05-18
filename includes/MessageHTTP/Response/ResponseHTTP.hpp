/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/16 15:14:46 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include "IMessageHTTP.hpp"

namespace WS {

class ResponseHTTP : public IMessageHTTP
{

	private:
		StatusLine			start_line;

	public:

		// constructor
		ResponseHTTP();
		ResponseHTTP(const ResponseHTTP & copy);

		// destructor
		virtual ~ResponseHTTP();

		// operator
		ResponseHTTP &	operator=(const ResponseHTTP & other);

		// functions







}; // end class RequestHTTP


} // end namespace

#endif
