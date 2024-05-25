/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:36:49 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/25 12:36:51 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_tab(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("declare -x %s\n", cmd[i]);
		i++;
	}
}

int	redir_or_not(char **av)
{
	int	i;
	int	j;

	i = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == '>' || av[i][j] == '<')
				return (1);
			if (av[i][j])
				j++;
		}
		i++;
	}
	return (0);
}

int	count_arg(char **cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		if (ft_strcmp("<", cmd[i]) == 0 || ft_strcmp(">", cmd[i]) == 0
			|| ft_strcmp("<<", cmd[i]) == 0 || ft_strcmp(">>", cmd[i]) == 0)
			i += 2;
		while (cmd[i] && (ft_strcmp(">", cmd[i]) != 0 && ft_strcmp(">>",
					cmd[i]) != 0 && ft_strcmp("<", cmd[i]) != 0
				&& ft_strcmp("<<", cmd[i]) != 0))
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**copy_tabcmd(t_set *set, char **cmd)
{
	char	**copy;
	int		i;
	int		j;
	int		size;

	size = count_arg(cmd);
	i = 0;
	j = 0;
	(void)set;
	if (size != 0)
		copy = (char **)malloc(sizeof(char *) * (size + 1));
	else
		return (NULL);
	while (cmd[i])
	{
		if (ft_strcmp("<", cmd[i]) == 0 || ft_strcmp(">", cmd[i]) == 0
			|| ft_strcmp("<<", cmd[i]) == 0 || ft_strcmp(">>", cmd[i]) == 0)
			i += 2;
		while (cmd[i] && (ft_strcmp(">", cmd[i]) != 0 && ft_strcmp(">>",
					cmd[i]) != 0 && ft_strcmp("<", cmd[i]) != 0
				&& ft_strcmp("<<", cmd[i]) != 0))
			copy[j++] = ft_strdup(cmd[i++]);
	}
	copy[j] = NULL;
	return (copy);
}
