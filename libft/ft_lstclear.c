/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:31:11 by retoriya          #+#    #+#             */
/*   Updated: 2024/04/22 22:26:32 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*tmp;

	if (!lst || !del || !*lst)
		return ;
	current = *lst;
	while (current != NULL)
	{
		del(current->content);
		tmp = current->next;
		free(current);
		current = tmp;
	}
	*lst = NULL;
}

void	delete_content(void *content)
{
	free(content);
}
