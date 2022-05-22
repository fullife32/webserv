/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DateFormat.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:56:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 14:37:11 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <ctime>
#include <iostream>
#include <cstring>

/*
	Date Format :  day-name "," SP date1 SP time-of-day SP GMT

	ex: Date: Wed, 18 May 2022 12:20:26 GMT

		date1 	= day SP month SP year
			day	   = 2DIGIT
			year   = 4DIGIT

		time-of-day = hour ":" minute ":" second
			hour   = 2DIGIT
			minute = 2DIGIT
			second = 2DIGIT

-----------------------------------------------------------------------------

	strftime: https://www.cplusplus.com/reference/ctime/strftime/

	a: Abbreviated weekday name *
	d: Day of the month, zero-padded (01-31)
	b: Abbreviated month name *
	Y: Year
	X: Time representation *
*/

std::string		getStringTime()
{
	time_t 		rawtime;
	struct tm * actual_time;
	char		buffer[BUFFER_TIME];

	time(&rawtime);
	actual_time = gmtime(&rawtime);

	strftime(buffer, BUFFER_TIME, "%a, %d %b %Y %X GMT", actual_time);
	return (std::string(buffer));
}
