/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:30:05 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/16 19:54:58 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "IMessageHTTP.hpp"
# include "RequestHTTP.hpp"

# define NEWLINE				"\n"
# define HEADER_SEPARATOR		"\r\n"
# define HEADER_SEPARATOR_SIZE	2
# define MESSAGE_END			"\r\n\r\n"

namespace WS
{

class ParseRequest
{

	private:
		std::string				m_data;
		std::string				m_header;
		std::string				m_body;
		std::string::iterator	m_header_begin; // NOPE finallement
		std::string::iterator	m_header_end;
		std::string::iterator	m_body_begin;
		std::string::iterator	m_body_end;


	public:

		ParseRequest(std::string	data);
		~ParseRequest();


		std::string &				append(const std::string & str); // pour recuperer la requete entiere ( buffer)
		RequestHTTP 				getFormated_RequestHTTP(); // fonction qui va appeler toutes les autres pour
									// fabriquer une RequestHTTP  throw une exception si probleme de parsing


	private:
		RequestLine					m_formated_RequestLine();	// formate la premiere ligne requestline 
		std::vector<std::string> &	m_splitRequestLine();		// split la premiere ligne par space ( )
		std::vector<std::string> &	m_getHeader();				// recupere le header en liste de string (split par \n)
		std::string &				m_getBody();				// recupere le body seul
		void						m_separateHeaderBody();

		// void						m_settingUpIterators();		// sépare avec les itérateurs le body du header




}; // end class

} // end namespace

#endif

