/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 18:58:49 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/14 13:54:35 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# define BUFFER_TIME	30

# include <string>
# include <vector>

std::vector<std::string>	splitString(const std::string & str, const std::string & separator);
std::vector<std::string>	splitStringtoTokens(char *str, const std::string & delimitors);

void			popLast( std::string & str );
void			popFirst( std::string & str );

std::string		extract_line(std::string & str, size_t	size, size_t pos = 0);
std::string		getStringTime();
size_t 			convertStringToSize(const std::string & str) ;

#endif
