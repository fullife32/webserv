/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 17:09:58 by rotrojan          #+#    #+#             */
/*   Updated: 2022/05/20 15:40:24 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket( int fd ): m_fd(fd) {}

Socket::~Socket() {}

void	Socket::closeSocket( void ) {
	if (m_fd != -1)
		close(m_fd);
}

int	const Socket::getFd( void ) const {
	return m_fd;
}