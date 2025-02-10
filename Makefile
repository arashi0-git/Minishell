# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/10 14:03:27 by aryamamo          #+#    #+#              #
#    Updated: 2025/02/10 16:23:42 by aryamamo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ./minishell
SRCDIR = ./srcs
SRCS = $(SRCDIR)/main.c\
		$(SRCDIR)/signal/signal.c\
		$(SRCDIR)/process/process_input.c\
		$(SRCDIR)/env/env.c\
		$(SRCDIR)/lexer/lexer.c\
		$(SRCDIR)/lexer/lexer_utils.c\
		$(SRCDIR)/parse/parse.c\
		$(SRCDIR)/parse/parse_handle.c\
		$(SRCDIR)/parse/parse_add_cmd.c\
		$(SRCDIR)/expand/expand.c\
		$(SRCDIR)/expand/expand_length.c\
		$(SRCDIR)/expand/expand_length_utils.c\
		$(SRCDIR)/expand/expand_perform.c\
		$(SRCDIR)/expand/expand_perform_utils.c\

##リリンク確認！

LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

OBJDIR = ./OBJ
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))


CC = cc
CFLAGS = -Wall -Wextra -Werror

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