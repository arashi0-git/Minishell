/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:27:11 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/04 14:34:34 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_node
{
	char			*word;
	struct s_node	*next;
}					t_node;

t_node	*create_node(const char *word)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
	{
		perror("malloc node failed");
		exit(EXIT_FAILURE);
	}
	new_node->word = strdup(word);
	if (new_node->word == NULL)
	{
		perror("strdup failed");
		exit(EXIT_FAILURE);
	}
	new_node->next = NULL;
	return (new_node);
}

void	free_list(t_node *head)
{
	t_node	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->word);
		free(tmp);
	}
}

int	main(void)
{
	t_node	*head;
	t_node	*current;
	int		found;

	found = 0;
	head = create_node("hello");
	head->next = create_node("world");
	head->next->next = create_node("aaa");
	head->next->next->next = create_node("Test");
	current = head;
	while (current != NULL)
	{
		if (strcmp(current->word, "|") == 0)
		{
			found = 1;
			break ;
		}
		printf("%s ", current->word);
		current = current->next;
	}
	printf("\n");
	if (!found)
		printf("Not found '|'\n");
	free_list(head);
	return (0);
}
