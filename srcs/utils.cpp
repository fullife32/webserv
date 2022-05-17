/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:56:58 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/17 14:48:25 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <map>
#include <iostream>

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


template <class T>
void	printVector(const std::vector<T> & vec)
{
	typename std::vector<T>::const_iterator	it;

	for (it = vec.begin(); it != vec.end(); it++)
		std::cout << *it << std::endl;
}

template <class T>
void	printMap(const std::map<T, T> & map)
{
	typename std::map<T, T>::const_iterator	it;

	for (it = map.begin(); it != map.end(); it++)
		std::cout << "key=" << (*it).first << " value=" << (*it).second << std::endl;
}