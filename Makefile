# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/10 14:03:27 by aryamamo          #+#    #+#              #
#    Updated: 2025/02/28 17:29:13 by retoriya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ./minishell
SRCDIR = ./srcs
SRCS = $(SRCDIR)/main.c \
		$(SRCDIR)/signal/signal.c \
		$(SRCDIR)/process/process_input.c \
		$(SRCDIR)/env/env.c \
		$(SRCDIR)/lexer/lexer.c \
		$(SRCDIR)/lexer/lexer_utils.c \
		$(SRCDIR)/parse/parse.c \
		$(SRCDIR)/parse/parse_handle.c \
		$(SRCDIR)/parse/parse_add_cmd.c \
		$(SRCDIR)/expand/expand.c \
		$(SRCDIR)/expand/expand_length.c \
		$(SRCDIR)/expand/expand_length_utils.c \
		$(SRCDIR)/expand/expand_perform.c \
		$(SRCDIR)/expand/expand_perform_utils.c \
		$(SRCDIR)/builtin/builtin.c \
		$(SRCDIR)/builtin/process_cd.c \
		$(SRCDIR)/builtin/process_echo.c \
		$(SRCDIR)/builtin/process_export.c \
		$(SRCDIR)/builtin/process_export_utils.c \
		$(SRCDIR)/builtin/process_export_utils2.c \
		$(SRCDIR)/builtin/process_pwd.c \
		$(SRCDIR)/builtin/process_unset.c \
		$(SRCDIR)/builtin/process_env.c\
		$(SRCDIR)/builtin/process_exit.c\
		$(SRCDIR)/execution/execute_command.c \
		$(SRCDIR)/execution/execute_binary.c \
		$(SRCDIR)/execution/execute_pipe.c \
		$(SRCDIR)/execution/find_commond_path.c \
		$(SRCDIR)/execution/redirect.c \
		$(SRCDIR)/execution/wait_for_commond.c\
		$(SRCDIR)/execution/utils.c \
		$(SRCDIR)/utils/free_array.c \
		$(SRCDIR)/utils/print_error.c
		
##リリンク確認！

LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

OBJDIR = ./OBJ
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))


CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LDFLAG = -lreadline

RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAG) -o $@

$(LIBFT):
	make -C $(LIBFTDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(filter %/$*.c, $(SRCS)) -o $@

clean:
	$(RM) $(OBJ)
	make -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFTDIR) fclean

re: fclean all
