/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:48:48 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/12 13:35:24 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHTTP.hpp"

/* -------------------------------------------------------------------------- */
/*                     Constructor Destructor                                 */
/* -------------------------------------------------------------------------- */

ParseRequest::ParseRequest(const ServerConf * server) :
	m_body(NULL),
	m_body_size(0),
	m_server(server),
	m_header_size(0),
	m_max_body_size(0),
	m_is_post_method(false),
	m_has_complete_header(false),
	m_has_complete_startLine(false)
{}


ParseRequest::ParseRequest(const ParseRequest & copy) :
	m_requestLine(copy.m_requestLine),
	m_body(copy.m_body),
	m_body_size(copy.m_body_size),
	m_server(copy.m_server),
	m_data(copy.m_data),
	m_header_size(copy.m_header_size),
	m_max_body_size(copy.m_max_body_size),
	m_is_post_method(copy.m_is_post_method),
	m_has_complete_header(copy.m_has_complete_header),
	m_has_complete_startLine(copy.m_has_complete_startLine)
{
	m_headerFields = copy.m_headerFields;
}

ParseRequest::~ParseRequest()
{
	clear();
}

void	ParseRequest::clear()
{
	m_requestLine.clear();
	if (m_body != NULL)
	{
		fclose(m_body);
		m_body = NULL;
	}
	m_body_size = 0;
	m_data.clear();
	m_header_size = 0;
	m_max_body_size = 0;
	m_is_post_method = false;
	m_has_complete_header = false;
	m_has_complete_startLine = false;
}

/* -------------------------------------------------------------------------- */

/*
**	append buffer from recv to complete the request data.

	as long as the header of the request is not complete try to parse header
	then
		if it is a POST method retrieve the rest of the request in the body
		otherwise if there is a body : request error
*/
void	ParseRequest::append(const std::string & buffer)
{
	if (m_has_complete_header == false)
	{
		m_header_size += buffer.size();
		m_data.append(buffer);
		if (m_parse_header() == false)
		{
			m_check_max_header_size();
			return ;
		}
		m_header_size -= m_data.size();
	}
	if (m_is_post_method)
	{
		if (m_data.empty() == false)
		{
			m_append_body(m_data);
			m_data.clear();
		}
		else 
			m_append_body(buffer);
	}
	else 
	{
		if (m_data.empty() == false)
			throw MessageErrorException(STATUS_BAD_REQUEST);
	}
}

void	ParseRequest::m_append_body(const std::string & buffer)
{
	if (fputs(buffer.data(), m_body) == EOF)
		throw MessageErrorException(STATUS_INTERNAL_SERVER_ERROR, m_requestLine.url);
	m_body_size += buffer.size();
	m_check_max_body_size();
}

bool	ParseRequest::empty() const 
{
	return (m_header_size == 0 && m_body_size == 0);
}

/* -------------------------------------------------------------------------- */

void	ParseRequest::m_prepare_POST_body()
{
	m_body = tmpfile();

	if (m_body == NULL)
		throw MessageErrorException(STATUS_INTERNAL_SERVER_ERROR, m_requestLine.url);
}

/*
	 get first line of Header
		formated the first line of Request Header :
		Method SP URL SP HTTPversion

		for now HTTPversion is facultatif
		throw exception (400 Bad Request)
*/
void 		ParseRequest::m_parse_RequestLine(const std::string & startline)
{
	std::vector<std::string>	split;

	split = splitString(startline, " ");
	if (split.size() < 2 || split.size() > 3)
		throw MessageErrorException(STATUS_BAD_REQUEST);

	if (split[1].size() > REQUEST_URL_MAX_SIZE)
		throw MessageErrorException(STATUS_URI_TOO_LONG);

	m_requestLine.method = split[0];
	m_parse_url(split[1]);
	if (split.size() == 3)
		m_requestLine.version.formatedVersion(split[2]);
	if (m_requestLine.method == "POST")
	{
		m_is_post_method = true ;
		m_prepare_POST_body();
		m_body_size = m_server->getBodySize(m_requestLine.url.serverName, m_requestLine.url.path);
	}
}



void	ParseRequest::m_parse_url(std::string target)
{
	size_t		found_fragment = target.find('#');
	size_t		found_query = target.find('?');
	size_t		found_file;
	size_t		found_extension;
	size_t		found_pathInfo;

	if (found_fragment != std::string::npos)
	{
		m_requestLine.url.fragment = std::string(&target[found_fragment + 1], &target[target.size()]);
		target.erase(found_fragment);
	}	
	if (found_query != std::string::npos)
	{
		m_requestLine.url.query = std::string(&target[found_query + 1], &target[target.size()]);
		target.erase(found_query);
		found_extension =  target.find_last_of('.');
		found_pathInfo = target.find('/', target.find_last_of('.'));
		if (found_pathInfo != std::string::npos)
		{
			m_requestLine.url.pathInfo = std::string(&target[found_pathInfo + 1], &target[target.size()]);
			target.erase(found_pathInfo);
		}
		
	}

	found_file = target.find_last_of('/');
	if (found_file != std::string::npos)
	{
		if (found_file == target.size())
			target.erase(found_file); 
		else 
		{
			found_extension = target.find_last_of('.');
			if (found_extension != std::string::npos && found_extension != target.size() && found_extension > found_file)
			{
				m_requestLine.url.fileExtension = std::string(&target[found_extension + 1], &target[target.size()]);
				m_requestLine.url.filename = std::string(&target[found_file + 1], &target[target.size()]);
				target.erase(found_file);
			}
		}
	}
	m_requestLine.url.path = target;
	if (m_requestLine.url.path.empty())
		m_requestLine.url.path = "/";
}


/*
**	get all header fields by separate each line of headerSplit by ':'
iterate each line of Header and create a map of < string, string >
corresponding to < HeaderFileds, HeaderFileds value >
*/
void 			ParseRequest::m_parse_headerFields(const std::string & line)
{
	size_t				separator;
	std::string			key;
	std::string			value;

	separator = line.find(":");
	if (separator == std::string::npos)
		throw MessageErrorException(100, m_requestLine.url);
	key = std::string(&line[0], &line[separator]);
	value = std::string(&line[separator + 1], &line[line.size()]);
	set_headerFields(key, value);
}


/*
	iterate each line for found startline and headerFields until a 
*/
bool			ParseRequest::m_parse_header() // parse raw data and check if header is complete
{
	size_t	new_line;

	while ((new_line = m_data.find(NEWLINE)) != std::string::npos)
	{
		if (new_line == 0) // end of Request or begining of Body
		{
			m_has_complete_header = true;
			m_data.erase(0, 2);
			// m_check_host_HeaderFields();
			return true ;
		}
		if (m_has_complete_startLine == false)	// first line is the startLine
		{
			m_parse_RequestLine(extract_line(m_data, new_line));
			m_data.erase(0, 2);
			m_has_complete_startLine = true;
		}
		else if (m_has_complete_header == false)
		{
			m_parse_headerFields(extract_line(m_data, new_line));
			m_data.erase(0, 2);
		}
		else
			return false ;
	}
	return false ;
}




void	ParseRequest::m_check_max_header_size() const
{
	if (m_header_size > REQUEST_HEADER_MAX_SIZE)
		throw MessageErrorException(STATUS_BAD_REQUEST); // TODO wich error ?
}

void	ParseRequest::m_check_max_body_size() const
{
	if (m_max_body_size != 0 && m_body_size > m_max_body_size)
		throw MessageErrorException(STATUS_PAYLOAD_TOO_LARGE); // TODO wich error ?
}


/*
	Check host for symplify search for opening file
		if not in headerfield : 
		else
			if already in target : do nothing
			else cat host + target in requestLine
*/
void	ParseRequest::m_check_host_HeaderFields(const std::string & url)
{
	// find Header Field "host"
	std::map<std::string, std::string>::iterator		found_host = m_headerFields.find(HF_HOST);
	
	if (found_host == m_headerFields.end()) ///// FAUT IL OBLIGATOIREMENT LE HOST ? normalement oui avec http1.1
		throw MessageErrorException(STATUS_BAD_REQUEST, m_requestLine.url);

	// get header field value of "host"
	m_requestLine.url.serverName = (*found_host).second;
	std::string		host = (*found_host).second;
	if (host.empty())
		throw MessageErrorException(STATUS_BAD_REQUEST, m_requestLine.url);

	// check if host is already in target
	if ( url.find(host) != std::string::npos)
	{
		m_requestLine.url.serverName = host;
		m_requestLine.url.path.erase(0, host.size());
	}
}
