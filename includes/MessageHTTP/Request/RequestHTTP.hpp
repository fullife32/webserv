/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:43:22 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/11 13:53:43 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

# include "utils.hpp"
# include "MessageStruct.hpp"
# include "ErrorMessage.hpp"
# include "ParseRequest.hpp"

class RequestHTTP : public MessageMethods, public ParseRequest
{
	/*
		protected variables herited from MessageMethods: list of all Methods
			static	std::map <std::string, int>		m_methods;

		protected variables herited from HeaderFields: list of all Methods
			std::map<std::string, std::string>	m_headerFields;

		protected variables herited from ParseRequest: list of all Methods
		RequestLine		m_requestLine;
		FILE *			m_body_tmp;

	*/
	private:
		std::string							m_body;
		std::string							m_path_body_for_CGI;

	public:

	/* constructor ------------------------------------------------ */
		RequestHTTP();
		RequestHTTP(const RequestHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~RequestHTTP();

	/* operator    ------------------------------------------------ */
		RequestHTTP &	operator=(const RequestHTTP & other);

	/* get / set    ------------------------------------------------ */
		int				getMethod() const;
		URL				getUrl() const;

	/* functions    ------------------------------------------------ */

		bool			hasQueryString() const ;
		bool			hasBody() const ;
		size_t			getBodySize() const ;
		FILE *			getBodyForCGI() const ;


}; // end class RequestHTTP

#endif
