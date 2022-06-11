/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_usefull.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:50:37 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/11 12:50:50 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"

std::string		extract_line(std::string & str, size_t	size)
{
	std::string		line(&str[0], &str[size]);

	str.erase(0, size);
	return line;
}
