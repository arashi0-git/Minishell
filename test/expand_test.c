/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:32:44 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/10 13:49:25 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// // #include "../../include/minishell.h"
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

typedef struct s_expand
{
	char			*out;
	size_t			i;
	size_t			out_index;
}					t_expand;

typedef struct s_shell
{
	int				exit_status;

}					t_shell;

static size_t	get_digit_count(int n)
{
	size_t	count;

	count = 0;
	if (n <= 0)
		count = 1;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	size_t			len;
	char			*result;
	unsigned int	num;

	len = get_digit_count(n);
	result = (char *)malloc(len + 1);
	if (result == NULL)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		num = -n;
	}
	else
		num = n;
	if (num == 0)
		result[0] = '0';
	result[len] = '\0';
	while (num != 0)
	{
		result[len - 1] = (num % 10) + '0';
		num = num / 10;
		len--;
	}
	return (result);
}

int	process_dollar_question(t_shell *shell, size_t *i, size_t *len)
{
	char	*status_str;

	status_str = ft_itoa(shell->exit_status);
	if (!status_str)
		return (-1);
	*len += strlen(status_str);
	free(status_str);
	*i += 2;
	return (0);
}

int	process_dollar_others(const char *str, size_t *i, size_t *len)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	(*i)++;
	var_start = *i;
	while (str[*i] && (isalnum((unsigned char)str[*i]) || str[*i] == '_'))
		(*i)++;
	var_len = *i - var_start;
	if (var_len == 0)
		(*len)++;
	else
	{
		var_name = strndup(&str[var_start], var_len);
		if (!var_name)
			return (-1);
		value = getenv(var_name);
		free(var_name);
		if (value)
			*len += strlen(value);
	}
	return (0);
}

int	process_single_quote(const char *str, size_t *i, size_t *len)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		(*len)++;
		(*i)++;
	}
	if (str[*i] != '\'')
		return (-1);
	(*i)++;
	return (0);
}

int	process_dollar_length(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	if (str[*i + 1] == '?')
		return (process_dollar_question(shell, i, len));
	else
		return (process_dollar_others(str, i, len));
}

int	process_double_quote(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	(*i)++;
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
		{
			if (process_dollar_length(str, shell, i, len) < 0)
				return (-1);
		}
		else
		{
			(*len)++;
			(*i)++;
		}
	}
	if (str[*i] != '"')
		return (-1);
	(*i)++;
	return (0);
}

int	process_quote_expand(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	int	length;

	length = 0;
	if (str[*i] == '\'')
		length = process_single_quote(str, i, len);
	else if (str[*i] == '"')
		length = process_double_quote(str, shell, i, len);
	return (length);
}

/*---------------------------------------------------------------------------------------------*/

/*                 expand_perform_utils.c                                                      */

/*---------------------------------------------------------------------------------------------*/
static int	expand_dollar_question(t_shell *shell, t_expand *exp)
{
	char	*status_str;
	size_t	t_len;

	status_str = ft_itoa(shell->exit_status);
	if (!status_str)
		return (-1);
	t_len = strlen(status_str);
	memcpy(&exp->out[exp->out_index], status_str, t_len);
	exp->out_index += t_len;
	free(status_str);
	exp->i += 2;
	return (0);
}

static int	copy_variable(const char *value, t_expand *exp)
{
	size_t	v_len;

	if (!value)
		value = "";
	v_len = strlen(value);
	memcpy(&exp->out[exp->out_index], value, v_len);
	exp->out_index += v_len;
	return (0);
}

static int	expand_dollar_variable(const char *str, t_expand *exp)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	exp->i++;
	var_start = exp->i;
	while (str[exp->i] && (isalnum((unsigned char)str[exp->i])
			|| str[exp->i] == '_'))
		exp->i++;
	var_len = exp->i - var_start;
	if (var_len == 0)
	{
		exp->out[exp->out_index++] = '$';
		return (0);
	}
	var_name = strndup(&str[var_start], var_len);
	if (!var_name)
		return (-1);
	value = getenv(var_name);
	free(var_name);
	return (copy_variable(value, exp));
}

/*---------------------------------------------------------------------------------------------*/

/*                    expand_perform.c                                                         */

/*---------------------------------------------------------------------------------------------*/

static int	expand_dollar(const char *str, t_shell *shell, t_expand *exp)
{
	if (str[exp->i + 1] == '?')
		return (expand_dollar_question(shell, exp));
	else
		return (expand_dollar_variable(str, exp));
}

static int	expand_single_quote(const char *str, t_expand *exp)
{
	exp->i++;
	while (str[exp->i] && str[exp->i] != '\'')
		exp->out[exp->out_index++] = str[exp->i++];
	if (str[exp->i] != '\'')
		return (-1);
	exp->i++;
	return (0);
}
static int	expand_double_quote(const char *str, t_shell *shell, t_expand *exp)
{
	exp->i++;
	while (str[exp->i] && str[exp->i] != '"')
	{
		if (str[exp->i] == '$')
		{
			if (expand_dollar(str, shell, exp) < 0)
				return (-1);
		}
		else
			exp->out[exp->out_index++] = str[exp->i++];
	}
	if (str[exp->i] != '"')
		return (-1);
	exp->i++;
	return (0);
}

int	process_expansion_char(const char *str, t_shell *shell, t_expand *exp)
{
	if (str[exp->i] == '\'')
	{
		if (expand_single_quote(str, exp) < 0)
			return (-1);
	}
	else if (str[exp->i] == '"')
	{
		if (expand_double_quote(str, shell, exp) < 0)
			return (-1);
	}
	else if (str[exp->i] == '$')
	{
		if (expand_dollar(str, shell, exp) < 0)
			return (-1);
	}
	else
		exp->out[exp->out_index++] = str[(exp->i)++];
	return (0);
}

/*---------------------------------------------------------------------------------------------*/

/*                      expand.c                                                               */

/*---------------------------------------------------------------------------------------------*/

t_expand	*init_expand(size_t total_len)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (NULL);
	exp->out = malloc(total_len + 1);
	if (!exp->out)
	{
		free(exp);
		return (NULL);
	}
	exp->i = 0;
	exp->out_index = 0;
	return (exp);
}

int	expand_length(const char *str, t_shell *shell)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (process_quote_expand(str, shell, &i, &len) < 0)
				return (-1);
		}
		else if (str[i] == '$')
		{
			if (process_dollar_length(str, shell, &i, &len) < 0)
				return (-1);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*perform_expansion(const char *str, t_shell *shell, size_t total_len)
{
	t_expand	*exp;
	char		*result;

	exp = init_expand(total_len);
	if (!exp)
		return (NULL);
	while (str[exp->i] != '\0')
	{
		if (process_expansion_char(str, shell, exp) < 0)
		{
			free(exp->out);
			free(exp);
			return (NULL);
		}
	}
	exp->out[exp->out_index] = '\0';
	result = exp->out;
	free(exp);
	return (result);
}

char	*expand(const char *str, t_shell *shell)
{
	int	total_len;

	total_len = expand_length(str, shell);
	if (total_len == -1)
	{
		shell->exit_status = 127;
		return (NULL);
	}
	return (perform_expansion(str, shell, total_len));
}

/*---------------------------------------------------------------------------------------------*/
/*                      parse_add_cmd.c                                                        */
/*---------------------------------------------------------------------------------------------*/

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		printf("malloc cmd failed\n");
		return (NULL);
	}
	cmd->command = NULL;
	cmd->max_args = 4;
	cmd->args = malloc(sizeof(char *) * cmd->max_args);
	if (!cmd->args)
	{
		printf("malloc args failed\n");
		return (NULL);
	}
	cmd->argc = 0;
	cmd->outfile = NULL;
	cmd->infile = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

static int	resize_args(t_cmd *cmd)
{
	int		i;
	int		new_size;
	char	**new_args;

	i = 0;
	new_size = cmd->max_args + 2;
	new_args = malloc(sizeof(char *) * new_size);
	if (new_args == NULL)
	{
		printf("malloc new_args failed\n");
		return (-1);
	}
	while (i < cmd->argc)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[cmd->argc] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	cmd->max_args = new_size;
	return (0);
}

static int	copy_arg_to_cmd(t_cmd *cmd, const char *arg)
{
	cmd->args[cmd->argc] = malloc(strlen(arg) + 1);
	if (cmd->args[cmd->argc] == NULL)
	{
		printf("malloc for arg copy failed\n");
		return (-1);
	}
	strcpy(cmd->args[cmd->argc], arg);
	cmd->argc++;
	cmd->args[cmd->argc] = NULL;
	return (0);
}

int	add_arg(t_cmd *cmd, const char *arg)
{
	if (cmd->argc + 1 >= cmd->max_args)
	{
		if (resize_args(cmd) != 0)
			return (-1);
	}
	return (copy_arg_to_cmd(cmd, arg));
}

/*---------------------------------------------------------------------------------------------*/
/*                      parse_handle.c                                                         */
/*---------------------------------------------------------------------------------------------*/

static int	handle_input_redirection(t_cmd *cmd, t_token *target)
{
	cmd->infile = malloc(strlen(target->value) + 1);
	if (!cmd->infile)
	{
		printf("malloc infile failed\n");
		return (-1);
	}
	strcpy(cmd->infile, target->value);
	return (0);
}

static int	handle_output_redirection(t_cmd *cmd, t_token *target,
		RedirectType redirType)
{
	cmd->outfile = malloc(strlen(target->value) + 1);
	if (cmd->outfile == NULL)
	{
		printf("malloc outfile failed\n");
		return (-1);
	}
	strcpy(cmd->outfile, target->value);
	if (redirType == REDIRECT_APPEND)
		cmd->append = 1;
	else
		cmd->append = 0;
	return (0);
}

int	handle_redirection(t_cmd *cmd, t_token **curr_ptr)
{
	int		ret;
	t_token	*redir;

	redir = *curr_ptr;
	*curr_ptr = (*curr_ptr)->next;
	if (*curr_ptr == NULL || (*curr_ptr)->type != TOKEN_COMMAND)
	{
		printf("Syntax error: redirection missing target\n");
		return (-1);
	}
	if (redir->redirType == REDIRECT_IN || redir->redirType == REDIRECT_HEREDOC)
	{
		ret = handle_input_redirection(cmd, *curr_ptr);
		if (ret != 0)
			return (ret);
	}
	else if (redir->redirType == REDIRECT_OUT
		|| redir->redirType == REDIRECT_APPEND)
	{
		ret = handle_output_redirection(cmd, *curr_ptr, redir->redirType);
		if (ret != 0)
			return (ret);
	}
	return (0);
}

/*---------------------------------------------------------------------------------------------*/
/*                      parse.c                                                                */
/*---------------------------------------------------------------------------------------------*/
t_cmd	*create_new_command(t_cmd **cmd_list, t_cmd **current_cmd)
{
	t_cmd	*new_cmd_ptr;
	t_cmd	*tail;

	new_cmd_ptr = new_cmd();
	if (!*cmd_list)
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

static int	handle_command_token(t_cmd *cmd, t_token *token)
{
	if (!cmd->command)
	{
		cmd->command = malloc(strlen(token->value) + 1);
		if (!cmd->command)
		{
			printf("malloc command failed\n");
			return (-1);
		}
		strcpy(cmd->command, token->value);
		if (add_arg(cmd, token->value) != 0)
		{
			printf("add_arg failed\n");
			return (-1);
		}
	}
	else
	{
		if (add_arg(cmd, token->value) != 0)
		{
			printf("add_arg failed\n");
			return (-1);
		}
	}
	return (0);
}

static int	process_token(t_token **curr_ptr, t_cmd **cmd_list,
		t_cmd **current_cmd)
{
	if (!*current_cmd)
		create_new_command(cmd_list, current_cmd);
	if ((*curr_ptr)->type == TOKEN_PIPE)
		*current_cmd = NULL;
	else if ((*curr_ptr)->type == TOKEN_REDIR)
	{
		if (handle_redirection(*current_cmd, curr_ptr) != 0)
			return (-1);
	}
	else if ((*curr_ptr)->type == TOKEN_COMMAND)
	{
		if (handle_command_token(*current_cmd, *curr_ptr) != 0)
			return (-1);
	}
	return (0);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_token	*curr;

	cmd_list = NULL;
	current_cmd = NULL;
	curr = tokens;
	while (curr)
	{
		if (process_token(&curr, &cmd_list, &current_cmd) != 0)
			return (NULL);
		curr = curr->next;
	}
	return (cmd_list);
}

/*---------------------------------------------------------------------------------------------*/
/*                      lexer_utils.c                                                          */
/*---------------------------------------------------------------------------------------------*/
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
		printf("error quote not close\n");
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
	{
		printf("redirect malloc failed\n");
		return (NULL);
	}
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

static char	*process_pipe(char **p)
{
	char	*token;

	token = malloc(2);
	if (!token)
	{
		printf("token malloc failed\n");
		return (NULL);
	}
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
	{
		printf("token malloc failed\n");
		return (NULL);
	}
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

/*---------------------------------------------------------------------------------------------*/
/*                      lexer.c                                                                */
/*---------------------------------------------------------------------------------------------*/

static t_token	*new_token_node(char *value, tokentype type,
		RedirectType redirType)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
	{
		printf("node malloc failed\n");
		return (NULL);
	}
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
	t_shell	shell;
	t_token	*token_list;
	t_cmd	*cmd_list;
	t_token	*tok;
	char	input[] = "echo 'Hello,World' \"$HOME/Desktop\" $USER $? | grep Desktop";
	char	*expanded;

	/* シェルの初期化（exit_status 例: 0） */
	shell.exit_status = 0;
	/* サンプル入力：
		*  例：echo コマンドで変数展開を含む引数を指定し、パイプで別コマンドに渡す
		*  シングルクォート内は展開されず、ダブルクォート内や非引用部は展開される
		*/
	printf("Original input: %s\n", input);
	/* 1. トークン化 */
	token_list = tokenize_list(input);
	if (!token_list)
	{
		write(STDERR_FILENO, "Tokenization error\n", 19);
		return (1);
	}
	/* 2. 各トークンに対して変数展開を実施（TOKEN_COMMAND と TOKEN_REDIR に適用） */
	tok = token_list;
	while (tok)
	{
		if (tok->type == TOKEN_COMMAND || tok->type == TOKEN_REDIR)
		{
			expanded = expand(tok->value, &shell);
			if (!expanded)
			{
				write(STDERR_FILENO, "Expansion error\n", 16);
				free_token_list(token_list);
				return (1);
			}
			free(tok->value);
			tok->value = expanded;
		}
		tok = tok->next;
	}
	/* 展開後のトークンリストを表示 */
	printf("=== Token List after Expansion ===\n");
	print_token_list(token_list);
	/* 3. パース処理 */
	cmd_list = parse_tokens(token_list);
	if (!cmd_list)
	{
		write(STDERR_FILENO, "Parsing error\n", 14);
		free_token_list(token_list);
		return (1);
	}
	/* パース結果を表示 */
	printf("\n=== Parsed Commands ===\n");
	print_cmd_list(cmd_list);
	/* 後始末 */
	free_cmd_list(cmd_list);
	free_token_list(token_list);
	return (0);
}
