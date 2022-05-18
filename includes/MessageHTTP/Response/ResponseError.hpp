/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseError.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 16:31:44 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/18 17:46:10 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEERROR_HPP
# define RESPONSEERROR_HPP

#include <map>
#include <string>

#define	S_RESPONSE_CONTINUE 			"Continue"
#define	S_RESPONSE_SWITCHING_PROTOCOL	"Switching Protocols"
#define	S_RESPONSE_OK					"OK"
#define	S_RESPONSE_CREATED				"Created"
#define	S_RESPONSE_ACCEPTED				"Accepted"
#define	S_RESPONSE_NON_AUTH_INFOS		"Non-Authoritative Information"
#define	S_RESPONSE_NO_CONTENT			"No Content"


enum e_error
{
	RESPONSE_CONTINUE				= 100,
	RESPONSE_SWITCHING_PROTOCOL		= 101,
	RESPONSE_OK						= 200,
	RESPONSE_CREATED				= 201,
	RESPONSE_ACCEPTED				= 202,
	RESPONSE_NON_AUTH_INFOS			= 203,
	RESPONSE_NO_CONTENT				= 204
};



class ResponseError
{

	std::map<int, std::string> error;

	ResponseError()
	{
		error[RESPONSE_CONTINUE] = S_RESPONSE_CONTINUE;
		error[RESPONSE_SWITCHING_PROTOCOL] = S_RESPONSE_SWITCHING_PROTOCOL;
		error[RESPONSE_OK] = S_RESPONSE_OK;
		error[RESPONSE_CREATED] = S_RESPONSE_CREATED;
		error[RESPONSE_ACCEPTED] = S_RESPONSE_ACCEPTED;
		error[RESPONSE_NON_AUTH_INFOS] = S_RESPONSE_NON_AUTH_INFOS;
		error[RESPONSE_NO_CONTENT] = S_RESPONSE_NO_CONTENT;
	}

	std::string &	operator[](int n_err)
	{
		return error[n_err];
	}

};


#endif
