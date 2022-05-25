/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:30:05 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/25 12:15:21 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "AMessageStruct.hpp"
# include <string>
# include <vector>
# include <map>


# define NEWLINE	"\n"
# define EMPTY_LINE	"\n\n"

namespace WS
{

class ParseRequest
{

	private: // make private
	public:
		std::string							m_data;
		std::string							m_header;
		std::string							m_body;
		RequestLine							m_requestLine;
		std::map<std::string, std::string>	m_headerFields;
	public:

	/* constructor ------------------------------------------------ */
		ParseRequest();
		ParseRequest(std::string	data);

	/* destructor  ------------------------------------------------ */
		~ParseRequest();


	/* functions    ------------------------------------------------ */
		std::string &	append(const std::string & str); // pour recuperer la requete entiere ( buffer)

		RequestLine		getRequestLine();
		std::string		getBody();
		std::map<std::string, std::string>	getHeaderFields();


	private:
	
		void			m_prepareRequestBuilding();
		void			m_separateHeaderBody(); // separe les datas dans m_header et m_body
		void			m_formated_HeaderFields(const std::vector<std::string> & headerSplit);// formate les headerfields
		void			m_formated_RequestLine(const std::string & startline);	// formate la premiere ligne requestline 
	
}; // end class

} // end namespace

#endif

