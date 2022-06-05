/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:30:05 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/05 14:52:02 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "MessageHTTP.hpp"

namespace WS
{

class ParseRequest : public HeaderFields
{

	private: // make private
		std::string							m_data;
		std::string							m_header;
		std::string							m_body;
		RequestLine							m_requestLine;
		// std::map<std::string, std::string>	m_headerFields;
	public:

	/* constructor ------------------------------------------------ */
		ParseRequest();
		ParseRequest(const ParseRequest & copy);

	/* destructor  ------------------------------------------------ */
		~ParseRequest();


	/* functions    ------------------------------------------------ */
		std::string &	append(const std::string & str); // pour recuperer la requete entiere ( buffer)

		RequestLine		getRequestLine();
		std::string		getBody();
		std::map<std::string, std::string>	getHeaderFields();

		void			m_prepareRequestBuilding();

	private:
	
		void			m_separateHeaderBody(); // separe les datas dans m_header et m_body
		void			m_formated_HeaderFields(const std::vector<std::string> & headerSplit);// formate les headerfields
		void			m_formated_RequestLine(const std::string & startline);	// formate la premiere ligne requestline 
		void			m_check_host_HeaderFields(const std::string & url);
		void			m_formated_Url(std::string url);
		
	public:

	//debug

	public:
		void			debug_print()
		{
			std::cout << "PARSE REQUEST DEBUG PRINT *******************************" << std::endl;
			std::cout << "HEADER: " <<  std::endl << m_data << std::endl;
			std::cout << "BODY: " <<  std::endl << m_body << std::endl;
			std::cout << "REQUESTLINE: " <<  std::endl;
			std::cout << "	method: " << m_requestLine.method << std::endl;
			std::cout << "	version: " << m_requestLine.version.name <<  m_requestLine.version.major_version << "." <<  m_requestLine.version.minor_version << std::endl;
			std::cout << "	servername: " << m_requestLine.url.serverName << std::endl;
			std::cout << "	path: " << m_requestLine.url.path << std::endl;
			std::cout << "	filename: " << m_requestLine.url.filename << std::endl;
			std::cout << "	extension: " << m_requestLine.url.fileExtension << std::endl;
			std::cout << "	query: " << m_requestLine.url.query << std::endl;
			std::cout << "	fragment: " << m_requestLine.url.fragment << std::endl;
		}

}; // end class

} // end namespace

#endif

