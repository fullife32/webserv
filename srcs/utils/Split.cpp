/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:49:59 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/22 15:33:22 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <vector>
#include <string>

/*
** Split version for std::string
** use std::vector for list all string from @str separate by @separator
*/

std::vector<std::string> 		splitString(const std::string & str, const std::string & separator)
{
	std::vector<std::string>	split;
	size_t						begin = 0;
	size_t						found = str.find(separator, begin);
	
	while (found != std::string::npos)
	{
		split.push_back(std::string(&str[begin], &str[found]));
		found += separator.size();
		begin = found;
		found = str.find(separator, found);
	}
	if (begin != str.size())		// split the end of str OR if separator never found
		split.push_back(std::string(&str[begin], &str[str.size()]));
	return split;
}
