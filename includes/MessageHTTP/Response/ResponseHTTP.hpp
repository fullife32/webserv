/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:42:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/20 16:10:24 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include "AMessageHTTP.hpp"

namespace WS {

class ResponseHTTP : public AMessageHTTP
{

	private:
		StatusLine			m_startLine;
		int					m_method;
		
	public:

	/* constructor ------------------------------------------------ */
		ResponseHTTP();
		ResponseHTTP(const ResponseHTTP & copy);

	/* destructor  ------------------------------------------------ */
		virtual ~ResponseHTTP();


	/* operator    ------------------------------------------------ */
		ResponseHTTP &	operator=(const ResponseHTTP & other);


	/* set		    ------------------------------------------------ */
	void		setRequestMethod(int method);
	/* functions    ------------------------------------------------ */



	private:

		void	m_minimalHeaderFields();
		void	m_parseMethod();

		void	m_method_GET();
		void	m_method_POST();
		void	m_method_DELETE();





}; // end class RequestHTTP


} // end namespace

#endif
