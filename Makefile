# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: julienbelda <julienbelda@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 12:09:05 by jubelda           #+#    #+#              #
#    Updated: 2025/08/12 09:28:07 by julienbelda      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER_DIRECTORY := ./includes
SRCS_DIRECTORY   := ./srcs
LIBFT_FOLDER     := $(SRCS_DIRECTORY)/libft
RL_PREFIX        := $(shell brew --prefix readline)

# Main file
MAIN_FILE        := main.c

# Parsing files
PARSING_FILES    := srcs/parsing/dynbuf.c \
                    srcs/parsing/env.c \
                    srcs/parsing/env2.c \
                    srcs/parsing/expand.c \
                    srcs/parsing/lexer.c \
                    srcs/parsing/lexer_helper.c \
                    srcs/parsing/parser.c \
                    srcs/parsing/parser2.c \
                    srcs/parsing/parser3.c \
                    srcs/parsing/signal.c \
                    srcs/parsing/signal2.c \
                    srcs/parsing/token.c

# Builtin files
BUILTIN_FILES    := srcs/execution/builtin/ft_cd.c \
                    srcs/execution/builtin/ft_echo.c \
                    srcs/execution/builtin/ft_env.c \
                    srcs/execution/builtin/ft_exit.c \
                    srcs/execution/builtin/ft_export.c \
                    srcs/execution/builtin/ft_pwd.c \
                    srcs/execution/builtin/ft_unset.c

# Execution files
EXECUTE_FILES    := srcs/execution/exec/exec_assign.c \
                    srcs/execution/exec/exec_launch.c \
                    srcs/execution/exec/exec_launch2.c \
                    srcs/execution/exec/exec_loop.c \
                    srcs/execution/exec/exec_loop2.c \
                    srcs/execution/exec/helper.c \
                    srcs/execution/exec/heredoc.c \
                    srcs/execution/exec/heredoc2.c \
                    srcs/execution/exec/path.c \
                    srcs/execution/exec/redirection.c

# Utils files
UTILS_FILES      := srcs/utils/env.c \
                    srcs/utils/env2.c \
                    srcs/utils/env_nodes.c \
                    srcs/utils/error.c \
                    srcs/utils/free.c

# All source files
SRCS := $(MAIN_FILE) $(PARSING_FILES) $(BUILTIN_FILES) $(EXECUTE_FILES) $(UTILS_FILES)
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
