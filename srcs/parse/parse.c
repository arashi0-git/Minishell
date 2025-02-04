/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:27:11 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/04 15:26:51 by aryamamo         ###   ########.fr       */
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

void	process_pipe(t_node *list)
{
	t_node	*curr;

	printf("process_pipe に渡されたリスト: ");
	curr = list;
	while (curr != NULL)
	{
		printf("%s ", curr->word);
		curr = curr->next;
	}
	printf("\n");
}

int	main(void)
{
	t_node	*head;
	t_node	*current;
	t_node	*new_head;
	t_node	*new_end;
	t_node	*new_node;

	new_head = NULL;
	new_end = NULL;
	head = create_node("hello");
	head->next = create_node("world");
	head->next->next = create_node("|");
	head->next->next->next = create_node("Test");
	head->next->next->next->next = create_node("111");
	head->next->next->next->next->next = create_node("|");
	head->next->next->next->next->next->next = create_node("222");
	head->next->next->next->next->next->next->next = create_node("333");
	current = head;
	while (current != NULL)
	{
		if (strcmp(current->word, "|") == 0)
		{
			if (new_head != NULL)
			{
				process_pipe(new_head);
				free_list(new_head);
				new_head = NULL;
				new_end = NULL;
			}
		}
		else
		{
			new_node = create_node(current->word);
			if (new_head == NULL)
			{
				new_head = new_node;
				new_end = new_node;
			}
			else
			{
				new_end->next = new_node;
				new_end = new_node;
			}
		}
		current = current->next;
	}
	if (new_head != NULL)
	{
		process_pipe(new_head);
		free_list(new_head);
	}
	free_list(head);
	return (0);
}
