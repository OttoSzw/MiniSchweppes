/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:27:47 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/20 23:27:50 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tab_calculate(char **s)
{
	int	i;

	i = 0;
	while (s[i] != NULL)
		i++;
	return (i);
}

int	tab_calculate_no_pipe(char **s)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (ft_strcmp("|", s[i]) == 0)
			i++;
		count++;
		i++;
	}
	return (count);
}

int	tab_calculate2(t_set *set, char **s)
{
	int	i;
	int	counter;

	counter = 0;
	i = set->index;
	counter = 0;
	if (ft_strcmp("|", s[i]) == 0)
		i++;
	while (s[i] && ft_strcmp("|", s[i]) != 0)
	{
		counter++;
		i++;
	}
	return (counter);
}

char	**copy_of_tab(char **tab)
{
	char	**copy;
	int		size;
	int		i;

	i = 0;
	size = (tab_calculate(tab) + 1);
	copy = (char **)ft_calloc(sizeof(char *), size);
	if (!copy)
		return (NULL);
	while (tab[i])
	{
		copy[i] = ft_strdup(tab[i]);
		if (!copy[i])
		{
			free_tab(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	***copy_of_tab_of_tab(t_set *set, char **tab)
{
	char	***copy;
	int		j;

	int (i) = 0;
	int (box) = 0;
	int (time) = 0;
	set->nb_arg = count_cmdpipe(tab);
	copy = (char ***)ft_calloc(sizeof(char **), (set->nb_arg + 1));
	while (tab[i] && time < (tab_calculate_no_pipe(tab)))
	{
		set->index = i;
		j = 0;
		set->nb_case = 0;
		set->nb_case = tab_calculate2(set, set->cmd);
		copy[box] = (char **)ft_calloc(sizeof(char *), (set->nb_case + 1));
		while (tab[i] && (ft_strcmp("|", tab[i]) != 0))
			copy[box][j++] = ft_strdup(tab[i++]);
		copy[box][j] = NULL;
		box++;
		if (tab[i])
			i++;
		time++;
	}
	copy[box] = NULL;
	return (copy);
}
