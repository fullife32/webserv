/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 10:38:45 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/22 15:30:01 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORMESSAGE_HPP
# define ERRORMESSAGE_HPP

# include <map>
# include <string>

#define	S_STATUS_CONTINUE 				"Continue"						// 100
#define	S_STATUS_SWITCHING_PROTOCOL		"Switching Protocols"			// 101
#define	S_STATUS_OK						"OK"							// 200
#define	S_STATUS_CREATED				"Created"						// 201
#define	S_STATUS_ACCEPTED				"Accepted"						// 202
#define	S_STATUS_NON_AUTH_INFOS			"Non-Authoritative_Information"	// 203
#define	S_STATUS_NO_CONTENT				"No Content" 					// 204
#define S_STATUS_RESET_CONTENT			"Reset Content"					// 205
#define S_STATUS_PARTIAL_CONTENT		"Partial Content"				// 206
#define S_STATUS_MULTIPLE_CHOICE		"Multiple Choices"				// 300
#define S_STATUS_MOVED_PERMANENTLY		"Moved Permanently"				// 301
#define S_STATUS_FOUND					"Found"							// 302
#define S_STATUS_SEE_OTHER				"See Other"						// 303
#define S_STATUS_NOT_MODIFIED			"Not Modified"					// 304
#define S_STATUS_USE_PROXY				"Use Proxy"						// 305
#define S_STATUS_TEMPORARY_REDIRECT		"Temporary Redirect"			// 307
#define S_STATUS_BAD_REQUEST			"Bad Request"					// 400
#define S_STATUS_UNAUTHORIZED			"Unauthorized"					// 401
#define S_STATUS_PAYMENT_REQUIRED		"Payment Required"				// 402
#define S_STATUS_FORBIDDEN				"Forbidden"						// 403
#define S_STATUS_NOT_FOUND				"Not Found"						// 404
#define S_STATUS_METHOD_NOT_ALLOWED		"Method Not Allowed"			// 405
#define S_STATUS_NOT_ACCEPTABLE			"Not Acceptable"				// 406
#define S_STATUS_PROXY_AUTHENTICATION_REQUIRED	"Proxy Authentication Required" // 407
#define S_STATUS_REQUEST_TIMEOUT		"Request Timeout"				// 408
#define S_STATUS_CONFLICT				"Conflict"						// 409
#define S_STATUS_GONE					"Gone"							// 410
#define S_STATUS_LENGHT_REQUIRED		"Length Required"				// 411
#define S_STATUS_PRECONDITION_FAILED	"Precondition Failed"			// 412
#define S_STATUS_PAYLOAD_TOO_LARGE		"Payload Too Large"				// 413
#define S_STATUS_URI_TOO_LONG			"URI Too Long"					// 414
#define S_STATUS_UNSUPPORTED_MEDIA_TYPE	"Unsupported Media Type"		// 415
#define S_STATUS_RANGE_NOT_SATISFIABLE	"Range Not Satisfiable"			// 416
#define S_STATUS_EXPECTATION_FAILED		"Expectation Failed"			// 417
#define S_STATUS_UPGRADE_REQUIRED		"Upgrade Required"				// 426
#define S_STATUS_INTERNAL_SERVER_ERROR	"Internal Server Error"			// 500
#define S_STATUS_NOT_IMPLEMENTED		"Not Implemented"				// 501
#define S_STATUS_BAD_GATEWAY			"Bad Gateway"					// 502
#define S_STATUS_SERVICE_UNVAILABLE		"Service Unavailable"			// 503
#define S_STATUS_GATEWAY_TIMEOUT		"Gateway Timeout"				// 504
#define S_STATUS_HTTPVERSION_NOT_SUPPORTED	"HTTP Version Not Supported"// 505


namespace WS 
{
	
enum e_statusCode
{
	STATUS_CONTINUE					= 100,
	STATUS_SWITCHING_PROTOCOL		= 101,
	STATUS_OK						= 200,
	STATUS_CREATED					= 201,
	STATUS_ACCEPTED					= 202,
	STATUS_NON_AUTH_INFOS			= 203,
	STATUS_NO_CONTENT				= 204,
	STATUS_RESET_CONTENT        	= 205,
	STATUS_PARTIAL_CONTENT			= 206,
	STATUS_MULTIPLE_CHOICE			= 300,
	STATUS_MOVED_PERMANENTLY		= 301,
	STATUS_FOUND					= 302,
	STATUS_SEE_OTHER				= 303,
	STATUS_NOT_MODIFIED				= 304,
	STATUS_USE_PROXY				= 305,
	STATUS_TEMPORARY_REDIRECT		= 307,
	STATUS_BAD_REQUEST				= 400,
	STATUS_UNAUTHORIZED 			= 401,
	STATUS_PAYMENT_REQUIRED			= 402,
	STATUS_FORBIDDEN				= 403,
	STATUS_NOT_FOUND				= 404,
	STATUS_METHOD_NOT_ALLOWED		= 405,
	STATUS_NOT_ACCEPTABLE			= 406,
	STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
	STATUS_REQUEST_TIMEOUT			= 408,
	STATUS_CONFLICT					= 409,
	STATUS_GONE						= 410,
	STATUS_LENGHT_REQUIRED			= 411,
	STATUS_PRECONDITION_FAILED		= 412,
	STATUS_PAYLOAD_TOO_LARGE		= 413,
	STATUS_URI_TOO_LONG				= 414,
	STATUS_UNSUPPORTED_MEDIA_TYPE	= 415,
	STATUS_RANGE_NOT_SATISFIABLE	= 416,
	STATUS_EXPECTATION_FAILED		= 417,
	STATUS_UPGRADE_REQUIRED			= 426,
	STATUS_INTERNAL_SERVER_ERROR	= 500,
	STATUS_NOT_IMPLEMENTED			= 501,
	STATUS_BAD_GATEWAY				= 502,
	STATUS_SERVICE_UNVAILABLE		= 503,
	STATUS_GATEWAY_TIMEOUT			= 504,
	STATUS_HTTPVERSION_NOT_SUPPORTED= 505
};
	
class MessageErrorException : public std::exception
{
	private:
		const int							m_current_error;

	public:
		static std::map<int, std::string> 	m_errors;

	/* constructor ------------------------------------------------ */
		MessageErrorException(int	error);

	/* functions    ----------------------------------------------- */
		int				getError() const throw() ;
		std::string		getMappedError() const throw() ;
};


std::map <int, std::string>		init_mapError()  throw() ;

}

#endif
