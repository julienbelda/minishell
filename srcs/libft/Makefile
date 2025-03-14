# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jubelda <jubelda@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/15 14:11:33 by julienbelda       #+#    #+#              #
#    Updated: 2025/03/14 12:26:59 by jubelda          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a

INCLUDES    = -I includes -I $(LIBFT_DIR)/includes/

LIBFT_DIR   = libft/

CC = gcc

CFLAGS =  -O3 # -Wall -Wextra -Werror # TODO remove comment

RM = rm -f

LDFLAGS = -L.

LDLIBS = -lft

HDDIRS = -I includes/

AR = ar rc

HEADER = libft.h

SRC = $(wildcard ft_strtok.c) \
$(wildcard *.c)

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean git norminette re