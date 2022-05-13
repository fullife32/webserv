/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessageHTTP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:22:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/13 15:01:03 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGEHTTP_HPP
# define IMESSAGEHTTP_HPP

# include <string>
# include <vector>
# include <map>

namespace WS {

/*
**	HTTP version = HTTP-name "/" DIGIT "." DIGIT
**	HTTP-name   = %x48.54.54.50 ; "HTTP", case-sensitive
*/

struct HTTPversion
{
	std::string		name;
	int				major_version;
	int				minor_version;
};

/*
** status-code    = 3DIGIT voir code erreur potentiellement une std::mqp <int, string> avec toute la liste d'erreur et 
** le reason_phrase correspondante ou ENUM ou define ??
*/

struct	StatusCode
{
	int				code;
	std::string 	*reason_phrase; // pointeur sur std::map<int, sting> status code error ? ou enum de code ?
};

/*
** Start line
*/

struct StartLine
{
	HTTPversion		version;
};


struct RequestLine : public StartLine
{
	std::string		target; // URL
	std::string		method; // ACTIONS : GET, POST, DELETE
};


struct StatusLine : public StartLine
{
	StatusCode		status;	
};


/*
** Interface pour les Request et Response.
*/


class IMessageHTTP 
{

	protected:
		StartLine								start_line;
		std::map < std::string, std::string > 	header_fields;
		std::vector<std::string>				body;


	public:
		// constructor
		IMessageHTTP();
		IMessageHTTP(const IMessageHTTP & copy);

		// destructor
		virtual ~IMessageHTTP();


		// operator
		IMessageHTTP &	operator=(const IMessageHTTP & other);


		// functions




}; // end class IMessageHTTP

} // end namespace


#endif
