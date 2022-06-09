/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP_formatting.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 13:51:21 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/09 14:11:17 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"

/* -------------------------------------------------------------------------- */
/*                     Formated Response                                      */
/* -------------------------------------------------------------------------- */
	
	/*
		Formated Response : 
			try openFile from URL
			formated Header : Status Line and Header Fields.
	*/
	void	ResponseHTTP::m_formated_Response()
	{
		m_header.clear();
		std::string	path = m_foundLocation();
		if (m_isAutoindex == false)
			m_openFile_Body(m_foundLocation());
		m_formated_StatusLine();
		m_formated_HeaderFields();
		if (m_isAutoindex == true)
			m_formated_autoIndex()
	}


	void	ResponseHTTP::m_formated_CGI_Response(const RequestHTTP & request)
	{
		std::cout << "there is a query string in the request" << std::endl;

		// try executeCGI(m_headerFields, *this, *m_server)
	}

	void	ResponseHTTP::m_formated_StatusLine()
	{
		m_header << START_LINE_HTTP_VERSION << SP << m_statusLine.statusCode << SP << m_statusLine.reasonPhrase << CRLF;
	}

	void	ResponseHTTP::m_formated_HeaderFields()
	{
		for (std::map<std::string, std::string>::iterator it = m_headerFields.begin(); it != m_headerFields.end(); it++)
			m_header << (*it).first << ": " << (*it).second << CRLF;
		m_header << CRLF;
	}



/* -------------------------------------------------------------------------- */
/*                     Formated Error Response                                */
/* -------------------------------------------------------------------------- */

	/*
		formated Error Response:
			create a simple body html for Error
	*/
	void	ResponseHTTP::m_formated_Error(const URL & url)
	{
		std::stringstream		body;
		std::string				ErrorUrl = m_server->getErrorPage(url.serverName, std::string(url.path), m_statusLine.statusCode);

		std::cout << "Error Url " <<  ErrorUrl << std::endl;
		std::cout << "Error Url path " <<  url.path << std::endl;

		m_formated_StatusLine();
		if (!ErrorUrl.empty())
		{
			ErrorUrl = url.path + ErrorUrl;
			std::cout << "Error Url " <<  ErrorUrl << std::endl;
			if (m_openFile_Error(ErrorUrl) == false)
				m_formated_ErrorBody(body);
		} 
		else
			m_formated_ErrorBody(body);
		m_formated_HeaderFields();

		m_header << body.str();
		std::cout << m_header.str() << std::endl;
	}


	void	ResponseHTTP::m_formated_ErrorBody(std::stringstream & body)
	{
		// if (ErrorUrl.empty())

		std::string	background_color;

		std::cout << "Formated Error Body" << std::endl;

		if (m_statusLine.statusCode >= 300 && m_statusLine.statusCode < 400)
			background_color = "\"background-color: bisque;";
		else if (m_statusLine.statusCode >= 400 && m_statusLine.statusCode < 500)
			background_color = "\"background-color: coral;";
		else
			background_color = "\"background-color: tomato;";

		body << "<!DOCTYPE html>" << CRLF;
		body << "<html lang=\"en\" style= " << background_color << ">\n";
		body << "<head>" << CRLF;
		body << "<meta charset=\"UTF-8\">" << CRLF;
		// body << "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">" << CRLF;
		body << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << CRLF;
		body << "<title>" << m_statusLine.statusCode << " " << m_statusLine.reasonPhrase << "</title>" << CRLF;
		body << "</head>" << CRLF;
		body <<  "<body style=\"font-size: xxx-large;font-family: monospace;text-align: -webkit-center; >\"; >" << CRLF;
		body << m_statusLine.statusCode << " " << m_statusLine.reasonPhrase << CRLF;
		body << "</body>" << CRLF;
		body << "</html>" << CRLF;
		body << CRLF;

		setContentLength(body.str().size());
		set_headerFields(HF_CONTENT_TYPE, "text/html");
	}
