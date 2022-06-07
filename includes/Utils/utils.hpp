/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:58:49 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/07 14:57:40 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>

std::vector<std::string> 		splitString(const std::string & str, const std::string & separator);
std::vector<std::string>		splitStringtoTokens(char *str, const std::string & delimitors);

void	popLast( std::string & str );
void	popFirst( std::string & str );

/*
** Printer
*/

# define BUFFER_TIME	30


std::vector<std::string>	splitString(const std::string & str, const std::string & separator);

std::string					getStringTime();

#endif
