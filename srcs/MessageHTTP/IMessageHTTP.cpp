#include "IMessageHTTP.hpp"

namespace WS 
{

	void	IMessageHTTP::setBody(const std::string & body)
	{
		m_body = body;
	}

	void	IMessageHTTP::setHeaderFields(const std::map<std::string, std::string> & headerFields)
	{
		m_header_fields = headerFields;
	}

} // end namespace
