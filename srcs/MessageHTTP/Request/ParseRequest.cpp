/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:48:48 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/15 12:07:40 by lvirgini         ###   ########.fr       */
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

/* -------------------------------------------------------------------------- */

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

bool	ParseRequest::empty() const 
{
	return (m_header_size == 0 && m_body_size == 0);
}

/* -------------------------------------------------------------------------- */

/*
**	append buffer from recv to complete the request data.

	as long as the header of the request is not complete try to parse header
	then
		if it is a POST method retrieve the rest of the request in the body
		otherwise if there is a body : request error
*/
void	ParseRequest::append(const std::string & buffer, char *buff, size_t size)
{
	size_t	size_processed = (size_t) -1;
	if (m_has_complete_header == false)
	{
		size_processed = found_header_end(buff, size);
		m_data.append(buffer);
		m_header_size += size_processed;
		if (m_parse_header() == false)
		{
			m_check_max_header_size();
			return ;
		}
	}
	if (m_has_complete_header && m_is_post_method)
	{
		if (size_processed != (size_t)-1)
		{
			m_save_body(buff, size_processed  + 4, size - size_processed - 4);
			m_body_size += size - size_processed - 4;
			m_data.clear();
		}
		else 
		{
			m_save_body(buff, 0, size);
			m_body_size += size;
		}
	}
	else 
	{
		if (m_data.empty() == false)
			throw MessageErrorException(STATUS_BAD_REQUEST);
	}
}

/* -------------------------------------------------------------------------- */

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
	std::cout << target << std::endl;

	m_parse_url_fragment(target);
	m_parse_url_query_string(target);
	m_parse_url_filename(target);
	m_parse_url_port(target);
	m_requestLine.url.path = target;
	if (m_requestLine.url.path.empty())
		m_requestLine.url.path = "/";
}

void		ParseRequest::m_parse_url_fragment(std::string & url)
{
	size_t	found_fragment = url.find('#');

	if (found_fragment != std::string::npos)
	{
		m_requestLine.url.fragment = std::string(&url[found_fragment + 1], &url[url.size()]);
		url.erase(found_fragment);
	}	
}

void	ParseRequest::m_parse_url_query_string(std::string & str)
{
	size_t		found_query = str.find('?');
	size_t		found_pathInfo;
	size_t		found_extension;

	if (found_query != std::string::npos)
	{
		m_requestLine.url.query = std::string(&str[found_query + 1], &str[str.size()]);
		str.erase(found_query);
		found_extension =  str.find_last_of('.');
		if (found_extension != std::string::npos)
		{
			found_pathInfo = str.find('/', str.find_last_of('.'));
			if (found_pathInfo != std::string::npos)
			{
				m_requestLine.url.pathInfo = std::string(&str[found_pathInfo + 1], &str[str.size()]);
				str.erase(found_pathInfo);
			}
		}
	}
}


void	ParseRequest::m_parse_url_filename(std::string & str)
{
	size_t		found_file = str.find_last_of('/');
	size_t		found_extension;

	if (found_file != std::string::npos)
	{
		if (found_file == str.size())
			str.erase(found_file); 
		else 
		{
			found_extension = str.find_last_of('.');
			if (found_extension != std::string::npos && found_extension != str.size() && found_extension > found_file)
			{
				m_requestLine.url.fileExtension = std::string(&str[found_extension + 1], &str[str.size()]);
				m_requestLine.url.filename = std::string(&str[found_file + 1], &str[str.size()]);
				str.erase(found_file);
			}
		}
	}
}


void	ParseRequest::m_parse_url_port(std::string & str)
{
	size_t	found_port = str.find(':');

	if (found_port != std::string::npos)
	{
		m_requestLine.url.port = std::string(&str[found_port + 1], &str[str.size()]);
		str.erase(found_port);
	}
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

	separator = line.find(':');
	if (separator == std::string::npos)
		throw MessageErrorException(STATUS_BAD_REQUEST, m_requestLine.url);
	key = std::string(&line[0], &line[separator]);
	value = std::string(&line[separator + 1], &line[line.size()]);
	if (value[0] == ' ')
		popFirst(value);
	set_headerFields(key, value);
}


/*
	iterate through each row to find the start row and header fields until an empty row is found.
	which means the end of the headers.
*/
bool			ParseRequest::m_parse_header()
{
	size_t	new_line;

	while ((new_line = m_data.find(NEWLINE)) != std::string::npos)
	{
		if (new_line == 0)
		{
			m_has_complete_header = true;
			m_data.erase(0, 2);
			m_check_host_HeaderFields();
			return true ;
		}
		if (m_has_complete_startLine == false)
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

void	ParseRequest::m_save_body(const char * buffer, size_t begin, size_t size)
{
	for (size_t i = 0; i < size; i++)
		fputc(buffer[begin + i], m_body);
}


void	ParseRequest::m_prepare_POST_body()
{
	m_body = tmpfile();

	if (m_body == NULL)
		throw MessageErrorException(STATUS_INTERNAL_SERVER_ERROR, m_requestLine.url);
}

/* -------------------------------------------------------------------------- */

void	ParseRequest::m_check_max_header_size() const
{
	if (m_header_size > REQUEST_HEADER_MAX_SIZE)
		throw MessageErrorException(STATUS_PAYLOAD_TOO_LARGE, m_requestLine.url);
}

void	ParseRequest::m_check_max_body_size() const
{
	if (m_max_body_size != 0 && m_body_size > m_max_body_size)
		throw MessageErrorException(STATUS_PAYLOAD_TOO_LARGE, m_requestLine.url);
}

size_t	ParseRequest::found_header_end(const char * buff, size_t size) const 
{
	for (size_t i = 0; i < (size - 4); i++)
	{
		if (buff[i] == '\r' && buff[i + 1] == '\n' && buff[i + 2] == '\r' && buff[i + 3] == '\n')
			return i;
	}
	return (size_t) -1;
}

/*
	Check host for symplify search for opening file
		if not in headerfield : 
		else
			if already in target : do nothing
			else cat host + target in requestLine
*/
void	ParseRequest::m_check_host_HeaderFields()
{
	std::map<std::string, std::string>::iterator		found_host = m_headerFields.find(HF_HOST);
	
	if (found_host == m_headerFields.end())
		throw MessageErrorException(STATUS_BAD_REQUEST, m_requestLine.url);

	std::string		host = (*found_host).second;
	m_requestLine.url.serverName = host;
	if (host.empty())
		throw MessageErrorException(STATUS_BAD_REQUEST, m_requestLine.url);

	if (m_requestLine.url.path.find(host) != std::string::npos)
	{
		m_requestLine.url.serverName = host;
		m_requestLine.url.path.erase(0, host.size());
	}
	
	size_t found_port = m_requestLine.url.serverName.find(':');
	if (found_port != std::string::npos)
	{
		m_requestLine.url.port = m_requestLine.url.serverName.substr(found_port + 1);
		m_requestLine.url.serverName.erase(found_port);
	}
}
