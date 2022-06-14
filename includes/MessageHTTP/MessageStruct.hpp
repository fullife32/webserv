/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageStruct.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 15:36:26 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/13 16:13:17 by rotrojan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGESTRUCT_HPP
# define MESSAGESTRUCT_HPP

#include <map>
#include <string>

#define	HF_HOST				"Host"
#define HF_CONTENT_LENGTH	"Content-Length"
#define HF_CONTENT_TYPE		"Content-Type"
#define HF_USER_AGENT		"User-Agent"
#define HF_LOCATION			"Location"
#define HF_SERVER			"Server"
#define HF_DATE				"Date"

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
	std::string	port;			//			::8080
	std::string	path;			//				/location/here
	std::string filename;		//								/file.html
	std::string	fileExtension;	//									 .html
	std::string	pathInfo;		//					if .cgi				.cgi/path/info
	std::string	query;			//													?queryString=value
	std::string fragment;		//																		#fragment


	URL() {};
	URL(const URL & other);
	URL & operator=(const URL & other);

			
	std::string		formatedPath() const ;
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


	public:
		typedef std::map<std::string, std::string> 	value_type;

		void				clear();
		void				set_headerFields(const std::string & headerField, const std::string & value);
		void				set_headerFields(const value_type & headerFlieds);

		std::string const	get_value_headerFields(const std::string & key) const ;
		HeaderFields const	&get_headerFields() const ;

	protected:
		value_type	m_headerFields;
};


/* Message Methods static map  --------------------------------- */

class MessageMethods
{
	protected:
		static	std::map <std::string, int>		m_methods;

};

std::map <std::string, int>		init_map_method();

/* Content Type  ------------------------------------------------ */


class ContentTypes
{
	protected:
		static std::map< std::string, std::string> m_listContentType;

	std::string get_contentType(const std::string & requestContentType) const ;
};

std::map< std::string, std::string>		init_map_ContentType();

#endif
