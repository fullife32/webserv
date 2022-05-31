/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:58:49 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/19 11:54:29 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_HPP
# define PRINT_HPP

# include <vector>
# include <map>

template <class T>
void	printMap(const std::map<T, T> & map);


template <class T>
void	printVector(const std::vector<T> & vec);

#endif
