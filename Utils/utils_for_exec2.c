/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_exec2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:02:02 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/25 15:02:04 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_next_line2(char **line)
{
	char	*buffer;

	int (i) = 0;
	int (r) = 0;
	char (c) = 0;
	buffer = (char *)calloc(10000, 1);
	if (!buffer)
		return (-1);
	r = read(0, &c, 1);
	if (r == 0)
	{
		free(buffer);
		*line = NULL;
		return (r);
	}
	while (r && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		r = read(0, &c, 1);
	}
	buffer[++i] = '\0';
	*line = buffer;
	return (r);
}

void	error_cmd(void)
{
	ft_putendl_fd("Command not found", 2);
	exit(EXIT_FAILURE);
}

int	error_mess(void)
{
	perror(NULL);
	exit(EXIT_FAILURE);
}

void	escape(char *path)
{
	free(path);
	error_mess();
}
