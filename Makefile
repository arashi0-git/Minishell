# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/08 23:08:08 by aryamamo          #+#    #+#              #
#    Updated: 2025/03/15 11:37:41 by aryamamo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ./minishell
SRCDIR = ./srcs
SRCS = $(SRCDIR)/main.c \
		$(SRCDIR)/signal/signal.c \
		$(SRCDIR)/process/process_input.c \
		$(SRCDIR)/process/process_utils.c \
		$(SRCDIR)/env/env.c \
		$(SRCDIR)/parse/parse.c \
		$(SRCDIR)/parse/parse_utils.c \
		$(SRCDIR)/parse/parse_handle.c \
		$(SRCDIR)/parse/parse_add_cmd.c \
		$(SRCDIR)/tokenize/tokenize.c\
		$(SRCDIR)/tokenize/tokenize_utils.c\
		$(SRCDIR)/expand/expand.c \
		$(SRCDIR)/expand/expand_length.c \
		$(SRCDIR)/expand/expand_length_utils.c \
		$(SRCDIR)/expand/expand_perform.c \
		$(SRCDIR)/expand/expand_perform_utils.c \
		$(SRCDIR)/expand/expand_split.c \
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
		$(SRCDIR)/execution/execute_command_utils.c \
		$(SRCDIR)/execution/execute_binary.c \
		$(SRCDIR)/execution/execute_pipe.c \
		$(SRCDIR)/execution/redirect_create.c \
		$(SRCDIR)/execution/redirect_operation.c \
		$(SRCDIR)/execution/redirect_operation2.c \
		$(SRCDIR)/execution/redirect_utils.c \
		$(SRCDIR)/execution/heredoc.c \
		$(SRCDIR)/execution/find_commond_path.c \
		$(SRCDIR)/execution/heredoc_utils.c \
		$(SRCDIR)/execution/wait_for_commond.c\
		$(SRCDIR)/execution/utils.c \
		$(SRCDIR)/utils/free_array.c \
		$(SRCDIR)/utils/print_error.c\
		$(SRCDIR)/utils/valid_identifier.c\
		$(SRCDIR)/utils/ft_getline.c 



LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

OBJDIR = ./OBJ
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))


CC = cc

CFLAGS = -Wall -Wextra -Werror -g -I./libft -I./include

LDFLAG = -lreadline

RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAG) -o $@

$(LIBFT):
	make -C $(LIBFTDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	make -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFTDIR) fclean

re: fclean all
