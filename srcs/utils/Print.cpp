/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 11:50:28 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 11:56:35 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include <iostream>
#include <vector>

template <class T>
void	printVector(const std::vector<T> & vec)
{
	typename std::vector<T>::const_iterator	it;

	for (it = vec.begin(); it != vec.end(); it++)
		std::cout << *it << std::endl;
}



template <class T, class U>
void	printMap(const std::map<T, U> & map)
{
	typename std::map<T, U>::const_iterator	it;

	for (it = map.begin(); it != map.end(); it++)
		std::cout << "key=" << (*it).first << " value=" << (*it).second << std::endl;
}
