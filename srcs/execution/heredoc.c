#include "../../include/minishell.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

t_bool	process_all_heredocs(t_cmd *cmd_list)
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
				if (!setup_heredoc_content(cmd, (char *)delim_node->content))
					return (FALSE);
				delim_node = delim_node->next;
			}
		}
		cmd = cmd->next;
	}
	return (TRUE);
}

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

t_bool	setup_heredoc_content(t_cmd *cmd, char *delimiter)
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
	redir->here_doc_content = read_until_delimiter(delimiter);
	if (!redir->here_doc_content)
	{
		free_redirect(redir);
		return (FALSE);
	}
	add_redirect_to_list(cmd, redir);
	return (TRUE);
}
