/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:30:05 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 17:11:18 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "AMessageHTTP.hpp"
# include "RequestHTTP.hpp"

# define NEWLINE	"\n"
# define EMPTY_LINE	"\n\n"

namespace WS
{

enum RequestMethod
{
	GET,
	POST,
	DELETE
};

class ParseRequest
{

	private: // make private
	public:
		std::string				m_data;
		std::string				m_header;
		std::string				m_body;

	public:

	/* constructor ------------------------------------------------ */
		ParseRequest();
		ParseRequest(std::string	data);

	/* destructor  ------------------------------------------------ */
		~ParseRequest();


	/* functions    ------------------------------------------------ */
		std::string &	append(const std::string & str); // pour recuperer la requete entiere ( buffer)
		RequestHTTP 	getFormated_RequestHTTP(); // fonction qui va appeler toutes les autres pour
						// fabriquer une RequestHTTP  throw une exception si probleme de parsing

	private:
	
		void								m_separateHeaderBody(); // separe les datas dans m_header et m_body
		std::map<std::string, std::string> 	m_formated_HeaderFields(const std::vector<std::string> & headerSplit);// formate les headerfields
		RequestLine							m_formated_RequestLine(const std::string & startline);	// formate la premiere ligne requestline 
		RequestMethod						m_formated_RequestMethod(const std::string & methodWord);



	public:
	
		class SyntaxException : public std::exception
		{
		private:
			int		m_error;
		
		public:
			 SyntaxException(int	error);
		
			virtual const char * what() const throw();
			int					getError() const ;
		};

		
}; // end class

} // end namespace

#endif

