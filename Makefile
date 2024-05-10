# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/20 23:27:37 by oszwalbe          #+#    #+#              #
#    Updated: 2024/04/29 14:08:00 by oszwalbe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c command.c
SRCS += Builtins/echo.c Builtins/cd.c Builtins/env.c Builtins/pwd.c Builtins/exit.c Builtins/unset.c Builtins/export.c
SRCS += Utils/utils_for_exec.c Utils/utils_for_tab.c Utils/utils_for_parse.c Utils/utils_for_pipe.c Utils/utils_for_files.c
SRCS += Utils/expand.c Utils/utils_for_expand.c Utils/utils_for_expand2.c
SRCS += $(wildcard pipex/*.c)
OBJS = ${SRCS:.c=.o}
LIBFTDIR = libft
LIBFTPATH = libft/libft.a


.c.o:
		${CC} ${CFLAGS} -g3 -c $< -o ${<:.c=.o}

${NAME}: ${OBJS} ${LIBFTPATH}
			${CC} -lreadline ${CFLAGS} ${OBJS} -L ${LIBFTDIR} -lft -o ${NAME}

${LIBFTPATH}:
				make -C ${LIBFTDIR}

all: ${NAME}

leaks : all
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --suppressions=ignore.txt -q ./minishell

clean:
		make -C ${LIBFTDIR} clean
		rm -f ${OBJS}

fclean: clean
		make -C ${LIBFTDIR} fclean
		rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
