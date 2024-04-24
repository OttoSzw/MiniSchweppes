/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:27:47 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/20 23:27:50 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tab(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

int	tab_calculate(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	**copy_of_tab(char **tab)
{
	char	**copy;
	int		size;
	int		i;

	i = 0;
	size = tab_calculate(tab) + 1;
	copy = (char **)malloc(sizeof(char *) * size);
	while (tab[i])
	{
		copy[i] = ft_strdup(tab[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
