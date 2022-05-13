/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 13:25:56 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/13 15:01:25 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include "ServerConfig.hpp"

namespace WS {

class Server 
{

	private:
	// liste de tous les servers
	std::vector<WS::ServerConfig>		servers;

	public:
		// constructor
		Server();
		Server(const Server & copy);

		// destructor
		~Server();


		// operator
		Server &	operator=(const Server & other);


		// functions

}; // end class Server

} // end namespace

#endif
