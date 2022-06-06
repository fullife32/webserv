/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageStruct.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:36:26 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/06 18:22:35 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGESTRUCT_HPP
# define MESSAGESTRUCT_HPP

#include "MessageHTTP.hpp"

namespace WS {


	/* HTTPversion  ------------------------------------------------ */
	/*
		HTTP version = HTTP-name "/" DIGIT "." DIGIT
		HTTP-name   = %x48.54.54.50 ; "HTTP", case-sensitive

		isSupportedVersion() is only 1.1 for our Webserv
	*/

	struct HTTPversion
	{
		std::string		name;
		int				major_version;
		int				minor_version;

		HTTPversion();
		~HTTPversion();
		
		void	formatedVersion(const std::string & version);
		bool	isSupportedVersion();
		void	clear();
	};

	/* URL						  ---------------------------------- */
	
	struct URL
	{
		std::string	serverName;		// example.com
		std::string	path;			//				/location/here
		std::string filename;		//								/file.html
		std::string	fileExtension;	//									 .html
		std::string	pathInfo;		//					if .cgi				.cgi/path/info
		std::string	query;			//													?queryString=value
		std::string fragment;		//																		#fragment

		std::string		formatedPath() const ; //TODO: maybe not
		void			clear();

	};

	/* Request Line -------m----------------------------------------- */


	struct RequestLine
	{
		HTTPversion		version;
		URL				url;
		std::string		method; // ACTIONS : GET, POST, DELETE

		void	clear();
	};


	/* Status Line  ------------------------------------------------ */
	/*
		status-code = 3DIGIT
		voir code erreur potentiellement une std::mqp <int, string> avec toute la liste d'erreur et 
		le reasonPhrase correspondante ou ENUM ou define ??
	*/

	struct StatusLine
	{
		HTTPversion		version;
		int				statusCode;
		std::string 	reasonPhrase;
	
		void	clear();
	};



	/* Header Fields  ------------------------------------------------ */

	class HeaderFields
	{
		typedef std::map<std::string, std::string> 	value_type;

		protected:
			value_type	m_headerFields;

		public:

			void			clear();
			void			set_headerFields(const std::string & headerField, const std::string & value);
			void			set_headerFields(const value_type & headerFlieds);

			std::string		get_value_headerFields(const std::string & key) const ;
			value_type		get_headerFields() const ;

	};
	

	/* Message Methods static map  --------------------------------- */

	class MessageMethods
	{
		protected:
			static	std::map <std::string, int>		m_methods;
	
	};
	
std::map <std::string, int>		init_map_method();


} //end namespace

#endif
