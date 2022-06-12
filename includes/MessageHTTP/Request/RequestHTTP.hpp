/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/12 13:58:21 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

# include "MessageHTTP.hpp"

class RequestHTTP : public MessageMethods, public ParseRequest
{
	/*
		protected variables herited from MessageMethods: list of all Methods
			static	std::map <std::string, int>	m_methods;

		protected variables herited from ParseRequest: list of all Methods
		std::map<std::string, std::string>		m_headerFields;
		RequestLine								m_requestLine;
		FILE *									m_body;				// body for CGI post
		size_t									m_body_size;		// size of body request
		size_t									m_max_body_size;
	*/

	public:

	/* constructor ------------------------------------------------ */
		RequestHTTP(const ServerConf * server);
		RequestHTTP(const RequestHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~RequestHTTP();

	/* operator    ------------------------------------------------ */
		RequestHTTP &	operator=(const RequestHTTP & other);

	/* get / set    ------------------------------------------------ */
		URL				getUrl() const;
		int				getMethod() const;
		size_t			getBodySize() const ;
		FILE *			getBodyForCGI() const ;

	/* functions    ------------------------------------------------ */

		bool			hasQueryString() const ;
		bool			hasBody() const ;

}; // end class RequestHTTP

#endif
