/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 10:42:57 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/20 16:03:03 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorMessage.hpp"

namespace WS
{

/* -------------------------------------------------------------------------- */
/*                               Exception                                    */
/* -------------------------------------------------------------------------- */

MessageErrorException::MessageErrorException(int error)
	: std::exception(),
	m_current_error(error)
{
	if (m_errors.empty())
		m_errors= init_mapError();
}

int				MessageErrorException::getError() const throw ()
{
	return m_current_error;
}

std::string		MessageErrorException::getMappedError() const throw ()
{
	return m_errors[m_current_error];
}


/* -------------------------------------------------------------------------- */

std::map <int, std::string>		init_mapError()  throw()
{
	std::map <int, std::string> error;

	error[STATUS_CONTINUE]		 		= S_STATUS_CONTINUE;				// 100
	error[STATUS_SWITCHING_PROTOCOL]	= S_STATUS_SWITCHING_PROTOCOL;		// 101
	error[STATUS_OK]					= S_STATUS_OK;						// 200
	error[STATUS_CREATED]				= S_STATUS_CREATED;					// 201	
	error[STATUS_ACCEPTED]				= S_STATUS_ACCEPTED;				// 202	
	error[STATUS_NON_AUTH_INFOS]		= S_STATUS_NON_AUTH_INFOS;			// 203
	error[STATUS_NO_CONTENT]			= S_STATUS_NO_CONTENT;				// 204
	error[STATUS_RESET_CONTENT]			= S_STATUS_RESET_CONTENT;			// 205
	error[STATUS_PARTIAL_CONTENT]		= S_STATUS_PARTIAL_CONTENT;			// 206
	error[STATUS_MULTIPLE_CHOICE]		= S_STATUS_MULTIPLE_CHOICE;			// 300
	error[STATUS_MOVED_PERMANENTLY]		= S_STATUS_MOVED_PERMANENTLY;		// 301
	error[STATUS_FOUND]		 			= S_STATUS_FOUND;					// 302
	error[STATUS_SEE_OTHER]				= S_STATUS_SEE_OTHER;				// 303
	error[STATUS_NOT_MODIFIED]			= S_STATUS_NOT_MODIFIED;			// 304
	error[STATUS_USE_PROXY]				= S_STATUS_USE_PROXY;				// 305
	error[STATUS_TEMPORARY_REDIRECT]	= S_STATUS_TEMPORARY_REDIRECT;		// 307
	error[STATUS_BAD_REQUEST]			= S_STATUS_BAD_REQUEST;				// 400
	error[STATUS_UNAUTHORIZED]			= S_STATUS_UNAUTHORIZED;			// 401
	error[STATUS_PAYMENT_REQUIRED]		= S_STATUS_PAYMENT_REQUIRED;		// 402
	error[STATUS_FORBIDDEN]				= S_STATUS_FORBIDDEN;				// 403	
	error[STATUS_NOT_FOUND]				= S_STATUS_NOT_FOUND;				// 404
	error[STATUS_METHOD_NOT_ALLOWED]	= S_STATUS_METHOD_NOT_ALLOWED;		// 405
	error[STATUS_NOT_ACCEPTABLE]		= S_STATUS_NOT_ACCEPTABLE;			// 406
	error[STATUS_PROXY_AUTHENTICATION_REQUIRED]	= S_STATUS_PROXY_AUTHENTICATION_REQUIRED; // 407
	error[STATUS_REQUEST_TIMEOUT]		= S_STATUS_REQUEST_TIMEOUT;			// 408
	error[STATUS_CONFLICT]				= S_STATUS_CONFLICT;				// 409
	error[STATUS_GONE]					= S_STATUS_GONE;					// 410
	error[STATUS_LENGHT_REQUIRED]		= S_STATUS_LENGHT_REQUIRED;			// 411
	error[STATUS_PRECONDITION_FAILED]	= S_STATUS_PRECONDITION_FAILED;		// 412
	error[STATUS_PAYLOAD_TOO_LARGE]		= S_STATUS_PAYLOAD_TOO_LARGE;		// 413
	error[STATUS_URI_TOO_LONG]			= S_STATUS_URI_TOO_LONG;			// 414
	error[STATUS_UNSUPPORTED_MEDIA_TYPE]= S_STATUS_UNSUPPORTED_MEDIA_TYPE;	// 415
	error[STATUS_RANGE_NOT_SATISFIABLE]	= S_STATUS_RANGE_NOT_SATISFIABLE; 	// 416
	error[STATUS_EXPECTATION_FAILED]	= S_STATUS_EXPECTATION_FAILED;		// 417
	error[STATUS_UPGRADE_REQUIRED]		= S_STATUS_UPGRADE_REQUIRED;		// 426
	error[STATUS_INTERNAL_SERVER_ERROR]	= S_STATUS_INTERNAL_SERVER_ERROR;	// 500
	error[STATUS_NOT_IMPLEMENTED]		= S_STATUS_NOT_IMPLEMENTED;			// 501
	error[STATUS_BAD_GATEWAY]			= S_STATUS_BAD_GATEWAY;				// 502
	error[STATUS_SERVICE_UNVAILABLE]	= S_STATUS_SERVICE_UNVAILABLE;		// 503
	error[STATUS_GATEWAY_TIMEOUT]		= S_STATUS_GATEWAY_TIMEOUT;			// 504
	error[STATUS_HTTPVERSION_NOT_SUPPORTED]	= S_STATUS_HTTPVERSION_NOT_SUPPORTED; // 505

	return error;
}



/* --- Init static map for error [status Code] = reason phrase ---------------- */

std::map <int, std::string>		WS::MessageErrorException::m_errors = init_mapError();


} // end namespace
