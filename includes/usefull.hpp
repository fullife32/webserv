/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:58:49 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/17 14:12:29 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USEFULL_HPP
# define USEFULL_HPP

std::vector<std::string> 		splitString(const std::string & str, const std::string & separator);


/*
** Printer
*/

template <class T>
void	printMap(const std::map<T, T> & map);


template <class T>
void	printVector(const std::vector<T> & vec);

#endif
