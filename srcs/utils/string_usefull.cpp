/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_usefull.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:50:37 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/14 19:58:36 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"
# include <sstream>
# include <iostream>

/*
	Use for Request and Response
*/

std::string		extract_line(std::string & str, size_t	size, size_t pos)
{
	std::string		line(&str[pos], &str[size]);

	str.erase(pos, size);
	return line;
}


size_t	convertStringToSize(const std::string & str)
{
	std::stringstream	s;
	size_t				size = 0;

	s << str;
	s >> size;
	return (size);
}

// std::string	convertStringToSize(size_t size)
// {
// 	std::stringstream	s;
// 	size_t				size = 0;

// 	s << str;
// 	s >> size;
// 	return (size);
// }