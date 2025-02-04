/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:40:48 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/04 17:49:58 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* --- トークン関連 --- */

typedef enum token_type
{
	TOKEN_COMMAND,
	TOKEN_PIPE,
	TOKEN_REDIR
}					tokentype;

typedef enum
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC
}					RedirectType;

typedef struct s_token
{
	char			*value;
	tokentype		type;
	RedirectType	redirType;
	struct s_token	*next;
}					t_token;

/* --- 単語（トークン）の処理 --- */

static char	*process_quote(char **p)
{
	char	quote;
	char	*start;
	int		len;
	char	*token;

	start = *p;
	quote = **p;
	(*p)++;
	while (**p && **p != quote)
		(*p)++;
	if (**p == '\0')
	{
		printf("error: quote not closed\n");
		return (NULL);
	}
	if (**p == quote)
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

static char	*process_redir(char **p)
{
	char	*start;
	char	c;
	int		len;
	char	*token;

	start = *p;
	c = **p;
	(*p)++;
	if (**p == c)
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

static char	*process_pipe(char **p)
{
	char	*token;

	token = malloc(2);
	if (!token)
		return (NULL);
	token[0] = **p;
	token[1] = '\0';
	(*p)++;
	return (token);
}

static char	*process_word(char **p)
{
	char	*start;
	int		len;
	char	*token;

	start = *p;
	while (**p && !isspace((unsigned char)**p) && **p != '\'' && **p != '"'
		&& **p != '>' && **p != '<' && **p != '|')
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

char	*get_token(char **p, tokentype *token_type)
{
	while (**p && isspace((unsigned char)**p))
		(*p)++;
	if (**p == '\0')
		return (NULL);
	if (**p == '\'' || **p == '"')
	{
		*token_type = TOKEN_COMMAND;
		return (process_quote(p));
	}
	else if (**p == '<' || **p == '>')
	{
		*token_type = TOKEN_REDIR;
		return (process_redir(p));
	}
	else if (**p == '|')
	{
		*token_type = TOKEN_PIPE;
		return (process_pipe(p));
	}
	else
	{
		*token_type = TOKEN_COMMAND;
		return (process_word(p));
	}
}

static t_token	*new_token_node(char *value, tokentype type,
		RedirectType redirType)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->redirType = redirType;
	node->next = NULL;
	return (node);
}

static RedirectType	redirect_type_from_token(const char *token)
{
	if (token[0] == '<')
	{
		if (strlen(token) == 1)
			return (REDIRECT_IN);
		else
			return (REDIRECT_HEREDOC);
	}
	else if (token[0] == '>')
	{
		if (strlen(token) == 1)
			return (REDIRECT_OUT);
		else
			return (REDIRECT_APPEND);
	}
	return (REDIRECT_OUT);
}

static t_token	*create_token_node(char *token, tokentype type)
{
	RedirectType	redirType;

	redirType = REDIRECT_OUT;
	if (type == TOKEN_REDIR)
		redirType = redirect_type_from_token(token);
	return (new_token_node(token, type, redirType));
}

static void	append_token_node(t_token **head, t_token **end, t_token *node)
{
	if (!*head)
	{
		*head = node;
		*end = node;
	}
	else
	{
		(*end)->next = node;
		*end = node;
	}
}

t_token	*tokenize_list(char *line)
{
	char		*p;
	char		*token;
	tokentype	type;
	t_token		*head;
	t_token		*end;
	t_token		*node;

	p = line;
	head = NULL;
	end = NULL;
	while ((token = get_token(&p, &type)) != NULL)
	{
		node = create_token_node(token, type);
		append_token_node(&head, &end, node);
	}
	return (head);
}

void	print_token_list(t_token *list)
{
	int	index;

	index = 0;
	while (list)
	{
		printf("Token[%d]: [%s] ", index, list->value);
		if (list->type == TOKEN_COMMAND)
			printf("Type: COMMAND");
		else if (list->type == TOKEN_PIPE)
			printf("Type: PIPE");
		else if (list->type == TOKEN_REDIR)
		{
			printf("Type: REDIR, ");
			if (list->redirType == REDIRECT_IN)
				printf("REDIRECT_IN");
			else if (list->redirType == REDIRECT_OUT)
				printf("REDIRECT_OUT");
			else if (list->redirType == REDIRECT_APPEND)
				printf("REDIRECT_APPEND");
			else if (list->redirType == REDIRECT_HEREDOC)
				printf("REDIRECT_HEREDOC");
		}
		printf("\n");
		index++;
		list = list->next;
	}
}

void	free_token_list(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->value);
		free(tmp);
	}
}

/*---parse---*/

typedef struct s_cmd
{
	char			*command;
	char			**args;
	int				argc;
	int				max_args;
	char			*infile;
	char			*outfile;
	int				append;
	struct s_cmd	*next;
}					t_cmd;

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		perror("malloc cmd failed");
		exit(EXIT_FAILURE);
	}
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->outfile = NULL;
	cmd->infile = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

int	add_arg(t_cmd *cmd, const char *arg)
{
	int		i;
	char	**new_args;
	int		new_size;

	if (cmd->argc + 1 >= cmd->max_args)
	{
		new_size = cmd->max_args + 2;
		new_args = malloc(sizeof(char *) * new_size);
		if (!new_args)
		{
			printf("malloc new_args failed\n");
			return (-1);
		}
		for (i = 0; i < cmd->argc; i++)
		{
			new_args[i] = cmd->args[i];
		}
		new_args[cmd->argc] = NULL;
		free(cmd->args);
		cmd->args = new_args;
		cmd->max_args = new_size;
	}
	cmd->args[cmd->argc] = malloc(strlen(arg) + 1);
	if (!cmd->args[cmd->argc])
	{
		printf("malloc for arg copy failed\n");
		return (-1);
	}
	strcpy(cmd->args[cmd->argc], arg);
	cmd->argc++;
	cmd->args[cmd->argc] = NULL;
	return (0);
}
static t_cmd	*create_new_commmand(t_cmd **cmd_list, t_cmd **current_cmd)
{
	t_cmd	*new_cmd_ptr;
	t_cmd	*tail;

	new_cmd_ptr = new_cmd();
	if (!*free_cmd_list)
		*cmd_list = new_cmd_ptr;
	else
	{
		tail = *cmd_list;
		while (tail->next)
			tail = tail->next;
		tail->next = new_cmd_ptr;
	}
	*current_cmd = new_cmd_ptr;
	return (new_cmd_ptr);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_token	*curr;
	t_cmd	*new_command;
	t_cmd	*tail;
	t_token	*redir;

	cmd_list = NULL;
	current_cmd = NULL;
	curr = tokens;
	while (curr)
	{
		if (!current_cmd)
		{
			new_command = new_cmd();
			if (!cmd_list)
				cmd_list = new_command;
			else
			{
				tail = cmd_list;
				while (tail->next)
					tail = tail->next;
				tail->next = new_command;
			}
			current_cmd = new_command;
		}
		if (curr->type == TOKEN_PIPE)
			current_cmd = NULL;
		else if (curr->type == TOKEN_REDIR)
		{
			redir = curr;
			curr = curr->next;
			if (!curr || curr->type != TOKEN_COMMAND)
			{
				printf("Syntax error: redirection missing target\n");
				return (cmd_list);
			}
			if (redir->redirType == REDIRECT_IN)
			{
				current_cmd->infile = malloc(strlen(curr->value) + 1);
				if (!current_cmd->infile)
				{
					printf("malloc infile failed\n");
					return (cmd_list);
				}
				strcpy(current_cmd->infile, curr->value);
			}
			else if (redir->redirType == REDIRECT_OUT)
			{
				current_cmd->outfile = malloc(strlen(curr->value) + 1);
				if (!current_cmd->outfile)
				{
					printf("malloc outfile failed\n");
					return (cmd_list);
				}
				strcpy(current_cmd->outfile, curr->value);
				current_cmd->append = 0;
			}
			else if (redir->redirType == REDIRECT_APPEND)
			{
				current_cmd->outfile = malloc(strlen(curr->value) + 1);
				if (!current_cmd->outfile)
				{
					printf("malloc outfile failed\n");
					return (cmd_list);
				}
				strcpy(current_cmd->outfile, curr->value);
				current_cmd->append = 1;
			}
			else if (redir->redirType == REDIRECT_HEREDOC)
			{
				current_cmd->infile = malloc(strlen(curr->value) + 1);
				if (!current_cmd->infile)
				{
					printf("malloc infile failed\n");
					return (cmd_list);
				}
				strcpy(current_cmd->infile, curr->value);
			}
		}
		else if (curr->type == TOKEN_COMMAND)
		{
			if (!current_cmd->command)
			{
				current_cmd->command = malloc(strlen(curr->value) + 1);
				if (!current_cmd->command)
				{
					printf("mallic command failed\n");
					return (cmd_list);
				}
				strcpy(current_cmd->command, curr->value);
				if (add_arg(current_cmd, curr->value) != 0)
				{
					printf("add_arg failed\n");
					exit(1);
				}
			}
			else
			{
				if (add_arg(current_cmd, curr->value) != 0)
				{
					printf("add_arg failed\n");
					exit(1);
				}
			}
		}
		curr = curr->next;
	}
	return (cmd_list);
}

void	print_cmd_list(t_cmd *cmd_list)
{
	int	i;
	int	j;

	i = 0;
	while (cmd_list)
	{
		j = 0;
		printf("Command %d:\n", i);
		if (cmd_list->command)
			printf("  Command: %s\n", cmd_list->command);
		if (cmd_list->argc > 0)
		{
			while (j < cmd_list->argc)
			{
				printf("  Arg[%d]: %s\n", j, cmd_list->args[j]);
				j++;
			}
		}
		if (cmd_list->infile)
			printf(" Infile: %s\n", cmd_list->infile);
		if (cmd_list->outfile)
			printf(" Outfile: %s(%s)\n", cmd_list->outfile,
				(cmd_list->append ? "append" : "overwrite"));
		cmd_list = cmd_list->next;
		i++;
	}
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;
	int		i;

	while (cmd_list)
	{
		tmp = cmd_list;
		cmd_list = cmd_list->next;
		if (tmp->args)
		{
			for (i = 0; i < tmp->argc; i++)
				free(tmp->args[i]);
			free(tmp->args);
		}
		if (tmp->command)
			free(tmp->command);
		if (tmp->infile)
			free(tmp->infile);
		if (tmp->outfile)
			free(tmp->outfile);
		free(tmp);
	}
}

/* --- テスト用 main --- */
int	main(void)
{
	char input[] = "cat -e ls  grep 'hello world'  wc -l << file2";
	t_token *token_list;
	t_cmd *cmd_list;

	/* 1. トークン化 */
	token_list = tokenize_list(input);
	printf("=== Token List ===\n");
	print_token_list(token_list);

	/* 2. パース */
	cmd_list = parse_tokens(token_list);
	printf("\n=== Parsed Commands ===\n");
	print_cmd_list(cmd_list);

	/* 後始末 */
	free_cmd_list(cmd_list);
	free_token_list(token_list);
	return (0);
}