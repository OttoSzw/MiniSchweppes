/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:55:43 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/12 14:55:45 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Ft_strnnn fonction qui check qu il n y ait que des "n" a la suite du '-';

int	ft_strnnn(char *s, char c)
{
	s++;
	while (*s)
	{
		if (*s != c)
			return (1);
		s++;
	}
	return (0);
}

int	echo_command(char **s)
{
	int	newline;
	int	len;

	newline = 1;
	if (*s == NULL)
	{
		write(1, "\n", 1);
		return (0);
	}
	if (ft_strncmp(*s, "-n", 2) == 0 && ft_strnnn(*s, 'n') == 0)
	{
		newline = 0;
		while (*s && ft_strncmp(*s, "-n", 2) == 0 && ft_strnnn(*s, 'n') == 0)
			s++;
		if (!(*s))
			return (0);
	}
	while (*s)
	{
		len = ft_strlen(*s);
		write(1, *s, len);
		if (*(s) && *(s + 1))
			write(1, " ", 1);
		s++;
	}
	if (newline == 1)
		write(1, "\n", 1);
	return (0);
}
