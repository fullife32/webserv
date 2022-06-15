/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 17:09:58 by rotrojan          #+#    #+#             */
/*   Updated: 2022/05/31 14:40:10 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket( int fd ): m_fd(fd) {}

Socket::~Socket() {}

void	Socket::closeSocket( void ) {
	if (m_fd != -1)
		close(m_fd);
}

int Socket::getFd( void ) const {
	return m_fd;
}
