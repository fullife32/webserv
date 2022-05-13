/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 13:22:35 by lvirgini          #+#    #+#             */
/*   Updated: 2022/05/13 15:02:18 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <vector>
# include <string>
# include <map>

namespace WS {

struct	ServerConfigLocation 
{
	std::string											location;	// location XXXX {...} dans le fichier de config
	std::map <std::string, std::vector <std::string> >	config;	// tous les parametres ou alors une variable par param ?

};


class ServerConfig 
{

	private: //

		// toute la config d'un bloc server
		std::vector<int>					port;		// ports listening
		std::vector<std::string>			server_name; // list of server_name
		std::string							root;		// dossier root mais il peut etre aussi dans chaque location
		std::vector<std::string>			index;		// pareil peut etre dans chaque location
		std::vector< ServerConfigLocation >	locations;	// list des structures location

	public:
		// constructor
		ServerConfig();
		ServerConfig(const ServerConfig & copy);

		// destructor
		~ServerConfig();


		// operator
		ServerConfig &	operator=(const ServerConfig & other);


		// functions



}; // end class ServerConfig

} // end namespace

#endif
