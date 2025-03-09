/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:29:47 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/07 04:26:06 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_getc(int fd, t_buffer *state)
{
	unsigned char	current_char;

	if (state->read_bytes == 0)
	{
		state->read_bytes = read(fd, state->buffer, BUFFER_SIZE);
		if (state->read_bytes == -1)
		{
			state->read_bytes = 0;
			return (READ_ERROR);
		}
		else if (state->read_bytes == 0)
			return (EOF);
		state->bufp = state->buffer;
	}
	current_char = *state->bufp;
	state->bufp++;
	state->read_bytes--;
	return (current_char);
}

int	ft_putc(t_string *line, char c)
{
	size_t	new_capa;
	char	*new_str;

	if (line->len + 1 >= line->capa)
	{
		new_capa = line->capa + BUFFER_SIZE;
		new_str = (char *)malloc(new_capa);
		if (!new_str)
			return (PUTC_ERROR);
		ft_memcpy(new_str, line->str, line->len);
		free(line->str);
		line->str = new_str;
		line->capa = new_capa;
	}
	line->str[line->len++] = c;
	line->str[line->len] = '\0';
	return (0);
}

char	*get_next_line(int fd)
{
	static t_buffer	buffer[OPEN_MAX];
	t_string		line;
	char			c;

	if (!(0 <= fd && fd <= OPEN_MAX && BUFFER_SIZE > 0))
		return (NULL);
	init_string(&line);
	while (1)
	{
		c = ft_getc(fd, &buffer[fd]);
		if (c == READ_ERROR)
		{
			free(line.str);
			return (NULL);
		}
		if (c == EOF || ft_putc(&line, c) == PUTC_ERROR)
			break ;
		if (c == '\n')
			return (line.str);
	}
	if (finalize_line(&line))
		return (NULL);
	return (line.str);
}

/*
int	main(void)
{
	int		fd1;
	char	*line1;

	fd1 = open("sample1.txt", O_RDONLY);
	//fd2 = open("sample2.txt", O_RDONLY);
	if (fd1 == -1)
	{
		printf("Failed to open files\n");
		return (1);
	}
	while (1)
	{
		line1 = get_next_line(fd1);
		//line2 = get_next_line(fd2);
		if (!line1) {
			break ;
		}
		if (line1)
		{
			printf("File1: %s", line1);
			free(line1);
		}
	}
	close(fd1);
	return (0);
}
__attribute__((destructor)) static void destructor(void)
{
	system("leaks -q a.out");
}
*/
