/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 10:29:39 by oszwalbe          #+#    #+#             */
/*   Updated: 2023/11/16 23:16:45 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*remplissage(int fd, char *stash)
{
	char	*buff;
	int		size;

	size = 1;
	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (NULL);
	while (check_new_line(stash) == 0 && size > 0)
	{
		size = read(fd, buff, BUFFER_SIZE);
		if (size < 0)
		{
			free(buff);
			return (NULL);
		}
		buff[size] = '\0';
		stash = ft_strjoin(stash, buff);
		if (!stash)
			return (NULL);
	}
	free(buff);
	return (stash);
}

char	*ft_cut(char *stash)
{
	char	*str;
	int		i;

	if (stash[0] == '\0')
	{
		free(stash);
		return (NULL);
	}
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	str = malloc((i + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = -1;
	while (stash[++i] && stash[i] != '\n')
		str[i] = stash[i];
	if (stash[i] == '\n')
		str[i++] = '\n';
	str[i] = '\0';
	return (str);
}

char	*ft_clean(char *stash)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	if (stash[0] == '\0')
	{
		free(stash);
		return (NULL);
	}
	while (stash[i] && stash[i] != '\n')
		i++;
	str = malloc((ft_strlen(stash) - i + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (stash[i] != '\0')
		i++;
	j = 0;
	while (stash[i])
		str[j++] = stash[i++];
	str[j] = '\0';
	free(stash);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*stash = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = remplissage(fd, stash);
	if (!stash)
		return (NULL);
	line = ft_cut(stash);
	if (line == NULL)
	{
		free(line);
		return (NULL);
	}
	stash = ft_clean(stash);
	return (line);
}

// int   main(int ac, char **av)
// {
//   char  *line;
//   int   fd1;
//   int   fd2;

// 	if (ac != 3)
// 		return (0);
//   fd1 = open(av[1], O_RDONLY);
//   fd2 = open(av[2], O_RDONLY);
//   line = get_next_line(fd1);
//   printf("%s\n", line);
//   line = get_next_line(fd1);
//   printf("%s\n", line);
//   line = get_next_line(fd2);
//   printf("%s\n", line);
//   line = get_next_line(fd2);
//   printf("%s\n", line);
//   line = get_next_line(fd1);
//   printf("%s\n", line);
//   line = get_next_line(fd2);
//   printf("%s\n", line);
//   line = get_next_line(fd1);
//   printf("%s\n", line);
//   line = get_next_line(fd2);
//   printf("%s\n", line);
//   return (0);
// }