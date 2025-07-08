# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: julienbelda <julienbelda@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 12:09:05 by jubelda           #+#    #+#              #
#    Updated: 2025/07/08 14:10:47 by julienbelda      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER_DIRECTORY := ./includes
SRCS_DIRECTORY   := ./srcs
LIBFT_FOLDER     := $(SRCS_DIRECTORY)/libft
RL_PREFIX        := $(shell brew --prefix readline)

MAIN_FILE        := main.c
PARSING_FILES    := $(wildcard $(SRCS_DIRECTORY)/parsing/*.c)
EXECUTING_FILES  := $(wildcard $(SRCS_DIRECTORY)/execution/*.c)
BUILTIN_FILES    := $(wildcard $(SRCS_DIRECTORY)/execution/builtin/*.c)
EXECUTE_FILES    := $(wildcard $(SRCS_DIRECTORY)/execution/exec/*.c)
LINKED_LIST_FILES:= $(wildcard $(SRCS_DIRECTORY)/linked_list/*.c)
UTILS_FILES := $(wildcard $(SRCS_DIRECTORY)/utils/*.c)

SRCS := $(MAIN_FILE) $(PARSING_FILES) $(EXECUTING_FILES) \
        $(BUILTIN_FILES) $(EXECUTE_FILES) $(LINKED_LIST_FILES) $(UTILS_FILES)
OBJS := $(SRCS:.c=.o)

CC       := gcc -g3 -fsanitize=address
CFLAGS   := -Wall -Wextra -Werror -O3 -g
INCFLAGS := -I$(HEADER_DIRECTORY) -I$(RL_PREFIX)/include
LIBFLAGS := -L$(RL_PREFIX)/lib -lreadline -lncurses

%.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_FOLDER)/libft.a
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_FOLDER) -lft $(LIBFLAGS) -o $@

$(LIBFT_FOLDER)/libft.a:
	$(MAKE) -C $(LIBFT_FOLDER)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_FOLDER) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_FOLDER) fclean

re: fclean all
.PHONY: all clean fclean re
