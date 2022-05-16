/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:56:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/16 19:35:16 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

std::vector<std::string> 		splitString(const std::string & str, const std::string & separator)
{
	std::vector<std::string>	split;
	size_t						begin = 0;
	size_t						size = str.find(separator, begin);
	
	while (size != std::string::npos)
	{
		split.push_back(std::string(str.at(begin), str.at(size)));
		size += separator.size();
		begin = size;
		size = str.find(separator, size);
	}
	return split;
}
