# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    settings.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lvirgini <lvirgini@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/17 16:36:27 by lvirgini          #+#    #+#              #
#    Updated: 2022/06/02 11:40:51 by lvirgini         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# The name of the binary
NAME = webserv

# The list of the all the sources, because $(wildcard *.c) is too dangerous
SRCS =	main_testRequest.cpp \
		AMessageHTTP.cpp \
		AMessageStruct.cpp \
		ParseRequest.cpp \
		RequestHTTP.cpp \
		ResponseHTTP.cpp \
		Split.cpp \
		ErrorMessage.cpp \
		DateFormat.cpp \
		Print.cpp \
		ServerConf.cpp \
		ParseConf.cpp \
		GetConf.cpp \
		Socket.cpp \
		Server.cpp \
		Multiplex.cpp \
		Client.cpp \
		utils.cpp \


# The file where make will look for all the sources (in addition to the root
# directory of the Makefile)
SRCS_DIR = srcs

# The libraries used
LIBS =

# The directory where all object and dependency files will be compiled
OBJS_DIR = .objs

# The file where make will look for all the header files
INC_DIR = includes

# The C++ compiler
CXX = clang++

# The C++ compilation flags
CXXFLAGS = -Wall -Wextra -std=c++98 -pedantic

# The characters and the length of the progress bar will adjust automatically
PROGRESS_BAR = -------------------------------------------------------------------
FILLING_CHAR = \#
