/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/23 15:43:15 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHTTP.hpp"

/*
	POUR LES METHOD : 405 NOT ALLOWED que ce soit une methode qui n'est pas autorisee
	ou qu'elle n'existe pas.


*/

namespace WS
{

	/*
		Construct a response from a requestHTTP
	*/
	ResponseHTTP::ResponseHTTP()
		: AMessageHTTP(),
		m_startLine(),
		m_method(0),
		m_dataResponse(),
		m_it_send(),
		m_chunk(0)
	{}


	ResponseHTTP::ResponseHTTP(const RequestHTTP & request)
		: AMessageHTTP(),
		m_startLine()
	{
		buildResponse(request);
	}


	ResponseHTTP::ResponseHTTP(const ResponseHTTP & copy)
	{
		*this = copy;
	}

	ResponseHTTP::~ResponseHTTP()
	{}


	ResponseHTTP & ResponseHTTP::operator=(const ResponseHTTP & other)
	{
		if (this != &other)
		{
			clear();
			m_startLine = other.m_startLine;
			m_header_fields = other.m_header_fields;
			m_body = other.m_body;
			m_method = other.m_method;
			m_dataResponse <<other.m_dataResponse;
			m_chunk = other.m_chunk;
		}
		return *this;
	}


	void	ResponseHTTP::buildResponse(const RequestHTTP & request)
	{
		clear();
		m_minimalHeaderFields();
		m_method = request.getMethod();
		m_parseMethod();
		m_formatedResponse();


	}


	void	ResponseHTTP::clear()
	{
		m_chunk = 0;
		m_method = 0;
		m_startLine.clear();
		m_body.clear();
		m_header_fields.clear();
		m_dataResponse.clear();

	}



	/*
		Set the minimals Header Fields needed for an answer.
	*/
	void	ResponseHTTP::m_minimalHeaderFields()
	{
		 m_header_fields["Date"] = getStringTime();
		 m_header_fields["Server"] = SERVER_NAME;
	}


	/*
		Setup the Method and Parse to the corresponding functions
		throw an exception if Method is not Allowed
	*/
	void	ResponseHTTP::m_parseMethod()
	{
		switch (m_method)
		{
			case (GET) : m_method_GET(); break;
			case (POST) : m_method_POST(); break;
			case (DELETE) : m_method_DELETE(); break;
			default :
				throw MessageErrorException(STATUS_METHOD_NOT_ALLOWED);
		}
	}


	void	ResponseHTTP::m_method_GET()
	{
		std::cout << "in methode GET" << std::endl;
	}
	void	ResponseHTTP::m_method_POST()
	{
		std::cout << "in methode POST" << std::endl;
	}
	void	ResponseHTTP::m_method_DELETE()
	{
		std::cout << "in methode DELETE" << std::endl;
	}



	void	ResponseHTTP::m_formatedResponse()
	{
		m_dataResponse << START_LINE_HTTP_VERSION << SP << m_startLine.status.code << SP << m_startLine.status.reasonPhrase;
		// std::cout << m_dataResponse.gcount() << std::endl;
		// std::cout <<  m_dataResponse.str().length() << std::endl;
		// std::cout <<  m_dataResponse.str() << std::endl;

		// std::cout << this->getNextSend(2) << std::endl;
	
	}

	size_t		ResponseHTTP::size() const
	{
		return m_dataResponse.str().size();
	}


	char *	ResponseHTTP::getNextChunk(size_t BufferSize)
	{

	}

		// write(1, m_dataResponse.str().data(), 2);

		// m_dataResponse.seekp(0);

		// std::cout << m_dataResponse.str() << std::endl;

		// m_dataResponse.seekp(5);


		// std::cout << m_dataResponse.str() << std::endl;

		// return (std::string());


		
		// if (m_chunk == 0)
		// 	m_it_send = m_dataResponse.str().begin();
		
		// std::string		toSend(m_it_send, m_it_send + BufferSize);
		// m_it_send += BufferSize;
		// m_chunk += BufferSize;
		// std::cout << *m_it_send << std::endl;
		// return (toSend);
	
	
		// if (m_chunk == 0)
		// 	ptr = m_dataResponse.str().at(0);

		// std::string
	
	
	}





} // end namespace



	// void	ResponseHTTP::m_formatedResponse()
	// {
	// 	m_dataResponse.append(START_LINE_HTTP_VERSION);
	// 	m_dataResponse.append(SP);
	// 	m_dataResponse.append(std::string(m_startLine.status.code));
	// 	m_dataResponse.append(SP);
	// 	m_dataResponse.append(m_startLine.status.reasonPhrase);
	// }
