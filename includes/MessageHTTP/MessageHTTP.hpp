/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHTTP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 11:24:35 by lvirgini          #+#    #+#             */
/*   Updated: 2022/06/03 13:38:58 by lvirgini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEHTTP_HPP
# define MESSAGEHTTP_HPP

# include "WebservDefine.hpp"

# define REQUEST_URL_MAX_SIZE	256
# define MESSAGE_BUFFER_SIZE	256
# define SERVER_NAME			"Webserv"
# define START_LINE_HTTP_VERSION "HTTP/1.1"
# define NEWLINE			"\n"
# define EMPTY_LINE			"\n\n"
# define CRLF				"\r\n"
# define SP					" "


#include <iostream>	// debug

# include <map>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>
# include <memory.h>
# include "Utils.hpp" // debug
# include "MessageStruct.hpp"
# include "ErrorMessage.hpp"
# include "ParseRequest.hpp"

# include "RequestHTTP.hpp"
# include "ResponseHTTP.hpp"



#endif
