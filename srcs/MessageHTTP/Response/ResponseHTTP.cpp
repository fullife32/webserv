/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHTTP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:34:27 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 11:45:32 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHTTP.hpp"

namespace WS
{

	ResponseHTTP::ResponseHTTP()
	: AMessageHTTP(),
	m_startLine()
	{
		m_minimalHeaderFields();
	}


	ResponseHTTP::~ResponseHTTP()
	{}




	void	ResponseHTTP::m_minimalHeaderFields()
	{
		// m_header_fields[]
	}







/*
** DATE formated to String for Response Header Fields :
**	 HTTP-date    = IMF-fixdate / obs-date
**
**	  Sun, 06 Nov 1994 08:49:37 GMT    ; IMF-fixdate

Struct tm {
    tm_sec int; // secondes de 0 à 61
    int tm_min; // minutes de 0 à 59
    int tm_hour; // heures de 0 à 24
    int tm_mday; // jour 1 à 31
    int tm_mon; // mois de 0 à 11
    int tm_year; // année depuis 1900
    int tm_wday; // jours depuis dimanche
    int tm_yday; // jours depuis le 1er janvier
    int tm_isdst; // heure d'été
}

*/

std::string		getStringDate()
{
	
}


} // end namespace


