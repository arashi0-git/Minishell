/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/10 06:21:36 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

static t_bool	setup_heredoc_token(t_token **token, char *delimiter)
{
	*token = malloc(sizeof(t_token));
	if (!*token)
		return (FALSE);
	(*token)->value = ft_strdup(delimiter);
	if (!(*token)->value)
	{
		free(*token);
		return (FALSE);
	}
	(*token)->type = TOKEN_COMMAND;
	(*token)->next = NULL;
	return (TRUE);
}

t_bool	setup_heredoc_content(t_cmd *cmd, char *delimiter, t_shell *shell)
{
	t_redirect	*redir;
	t_token		*token;

	if (!setup_heredoc_token(&token, delimiter))
		return (FALSE);
	redir = create_redirect(REDIRECT_HEREDOC, token, STDIN_FILENO);
	if (!redir)
	{
		free_token_list(token);
		return (FALSE);
	}
	redir->here_doc_content = read_until_delimiter(delimiter, shell);
	if (!redir->here_doc_content)
	{
		free_redirect(redir);
		return (FALSE);
	}
	add_redirect_to_list(cmd, redir);
	return (TRUE);
}

t_bool	process_all_heredocs(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*cmd;
	t_list	*delim_node;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_flag && cmd->heredoc_delims)
		{
			delim_node = cmd->heredoc_delims;
			while (delim_node)
			{
				if (!setup_heredoc_content(cmd, (char *)delim_node->content,
						shell))
					return (FALSE);
				delim_node = delim_node->next;
			}
		}
		cmd = cmd->next;
	}
	return (TRUE);
}

char	*read_until_delimiter(char *delimiter, t_shell *shell)
{
	char	*content;
	char	*line;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = get_expanded_line(shell);
		if (!line)
			break ;
		if (process_delimiter(line, delimiter, &content))
		{
			free(line);
			break ;
		}
		if (!append_to_content(&content, line))
		{
			free(line);
			free(content);
			return (NULL);
		}
		free(line);
	}
	return (content);
}

int	handle_heredoc(t_redirect *redir)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		print_error(strerror(errno), NULL, "heredoc");
		return (-1);
	}
	if (redir->here_doc_content)
	{
		ft_putstr_fd(redir->here_doc_content, pipe_fd[1]);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
