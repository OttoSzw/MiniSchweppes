/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:25:54 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:25:55 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_append(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strcmp(">>", cmd[i]) == 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_redirections(t_set *set, char **av)
{
	int	j;

	int (i) = set->index2;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if ((av[i][j] == '>' && av[i][j + 1] && av[i][j + 1] != '>')
				|| (av[i][j] == '>' && !av[i][j + 1]))
				return (set_index2(set, i), 2);
			else if ((av[i][j] == '>' && av[i][j + 2] && av[i][j + 1] == '>')
				|| (av[i][j] == '>' && !av[i][j + 2] && av[i][j + 1] == '>'))
				return (set_index2(set, i), 4);
			else if ((av[i][j] == '<' && av[i][j + 1] && av[i][j + 1] != '<')
				|| (av[i][j] == '<' && !av[i][j + 1]))
				return (set_index2(set, i), 1);
			else if ((av[i][j] == '<' && av[i][j + 2] && av[i][j + 1] == '<')
				|| (av[i][j] == '<' && !av[i][j + 2] && av[i][j + 1] == '<'))
				return (set_index2(set, i), 3);
			j++;
		}
		i++;
	}
	return (0);
}

int	count_nb_files(char **av)
{
	int	i;
	int	counter;

	counter = 0;
	i = 0;
	while (av[i])
	{
		if (ft_strcmp(">", av[i]) == 0)
			counter++;
		else if (ft_strcmp(">>", av[i]) == 0)
			counter++;
		else if (ft_strcmp("<", av[i]) == 0)
			counter++;
		else if (ft_strcmp("<<", av[i]) == 0)
			counter++;
		i++;
	}
	return (counter);
}

char	*find_file_out2(t_set *set, char **cmd)
{
	int	i;

	i = set->index;
	if (!cmd)
		return (NULL);
	while (cmd[i])
	{
		if ((ft_strcmp(">", cmd[i]) == 0) || (ft_strcmp(">>", cmd[i]) == 0)
			|| (ft_strcmp("<", cmd[i]) == 0) || (ft_strcmp("<<", cmd[i]) == 0))
			return (set_index(set, i), cmd[i + 1]);
		if (cmd[i])
			i++;
	}
	return (NULL);
}
