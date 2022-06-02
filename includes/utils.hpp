/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:58:49 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/28 19:13:37 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

std::vector<std::string> 		splitString(const std::string & str, const std::string & separator);
std::vector<std::string>		splitStringtoTokens(char *str, const std::string & delimitors);

void	popLast( std::string & str );
void	popFirst( std::string & str );

/*
** Printer
*/

template <class T>
void	printMap(const std::map<T, T> & map);


template <class T>
void	printVector(const std::vector<T> & vec);

#endif
