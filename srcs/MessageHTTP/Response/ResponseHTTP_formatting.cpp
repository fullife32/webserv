/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP_formatting.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 13:51:21 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/12 22:26:36 by lvirgini         ###   ########.fr       */
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
		std::string path = m_foundLocation();
		if (m_is_redirection == true)
		{
			m_formated_StatusLine();
			m_formated_HeaderFields();
			return ;
		}

		if (m_isAutoindex == false) {
			m_openFile_Body(path);
			m_formated_StatusLine();
			m_formated_HeaderFields();
		}
		else if (m_isAutoindex == true) {
			m_formated_Autoindex(path); }
	}


	void	ResponseHTTP::m_formated_CGI_Response(const RequestHTTP & request)
	{
		std::cout << "this is a CGI request" << std::endl;

		m_openFile_CGI();	// creation du fichier temporaire pour la reponse a envoyer au client

		/*
		 	dans le CGI faire :

			int fd_IN = fileno(request.getBodyForCGI());
			int fd_OUT = fileno(response.getBodyForCGI());
		*/


		// DEBUG : ///////////////////////////////////////////////////////////////////
		FILE * request_body = request.getBodyForCGI();
		if (request_body != NULL)
		{

			std::cout << "xxxxxxxxxxxxxxxx REQUEST BODY: xxxxxxxxxxxxxxxxx " << std::endl;
			char buffer[1000] = {0};
			while (!feof(request_body))
			{
				if (fgets (buffer,1000,request_body) == NULL) break;
				std::cout << buffer << std::endl;
			}
		}
		// DEBUG : ///////////////////////////////////////////////////////////////////

		m_setCGIBodySize();
		m_formated_StatusLine();
		m_formated_HeaderFields();
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

	void	ResponseHTTP::m_formated_Autoindex(std::string &path ) // TODO added by Eithan
	{
		std::string			actualPath(m_url.path.begin() + 1, m_url.path.end());
		std::stringstream	body;

		glob_t glob_result;
		glob_result.gl_offs = 2;

		int return_value = ::glob((path + '*').c_str(), GLOB_TILDE, NULL, &glob_result);

		if (return_value != 0 && return_value != GLOB_NOMATCH)
			throw MessageErrorException(STATUS_FORBIDDEN, m_url);
		std::vector<std::string> filenames(glob_result.gl_pathv,
			glob_result.gl_pathv + glob_result.gl_pathc);
		globfree(&glob_result);

		body << "<!DOCTYPE html>" << CRLF;
		body << "<html lang=\"en\">" << CRLF;
		body << "<head>" << CRLF;
		body << "<meta charset=\"UTF-8\">" << CRLF;
		body << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << CRLF;
		body << "</head>" << CRLF;
		body << "<body style=\"font-size: x-large;font-family: monospace;text-align: -webkit-left; >\"; >" << CRLF;
		body << "<h3>Index of /" << actualPath << "</h3>" << CRLF;
		body << "<a href=\"/\">/</a>" << CRLF;
		if (return_value != GLOB_NOMATCH) {
			for (std::vector<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
				body << "<br><a href=\"" <<  actualPath + "/" + (*it).erase(0, path.size()) << "\">/" << (*it) << "</a>" << CRLF;
		}
		body << "</body>" << CRLF;
		body << "</html>" << CRLF;
		body << CRLF << CRLF;

		setContentLength(body.str().size());
		set_headerFields(HF_CONTENT_TYPE, "text/html");
		m_formated_StatusLine();
		m_formated_HeaderFields();
		m_header << body.str();
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

		m_formated_StatusLine();
		if (!ErrorUrl.empty())
		{
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
