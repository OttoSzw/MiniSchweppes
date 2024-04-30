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

int	tab_calculate_no_pipe(char **s)
{
	int	i;
	int count;

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
	int counter;

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

int	count_cmdpipe(char **av)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (av[i])
	{
		if (ft_strcmp("|", av[i]) == 0)
			counter++;
		i++;
	}
	counter++;
	return (counter);
}

char	***copy_of_tab_of_tab(t_set *set, char **tab)
{
	char ***copy;
	int nb_arg;
	int	nb_case;
	int i;
	int box;
	int j;
	int	time;

	nb_arg = count_cmdpipe(tab);
	i = 0;
	box = 0;
	time = 0;
	copy = (char ***)ft_calloc(sizeof(char **), (nb_arg + 1));
	while (tab[i] && time < (tab_calculate_no_pipe(tab)))
	{
		set->index = i;
		j = 0;
		nb_case = 0;
		nb_case = tab_calculate2(set, set->cmd);
		copy[box] = (char **)ft_calloc(sizeof(char *), (nb_case + 1));
		while (tab[i] && (ft_strcmp("|", tab[i]) != 0))
		{
			copy[box][j] = ft_strdup(tab[i]);
			j++;
			i++;
		}
		copy[box][j] = NULL;
		box++;
		if (tab[i])
			i++;
		time++;
	}
	printf("%d\n", time);
	copy[box] = NULL;
	return (copy);
}