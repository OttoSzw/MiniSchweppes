/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:28:14 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/20 23:28:16 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_var_of_env(t_set *set)
{
	int	i;
	int	j;
	char	cwd[1024];
	char	*index;
	char	*content;

	i = 0;
	content = ft_strdup(getcwd(cwd, sizeof(cwd)));
	while (set->env[i])
	{
		j = 0;
		if (ft_strncmp("PWD=", set->env[i], 4) == 0)
		{
			index = malloc(sizeof(char) * 5);
			while (set->env[i][j] != '=')
			{
				index[j] = set->env[i][j];
				j++;
			}
			index[j] = set->env[i][j];
			j++;
			index[j] = '\0';
			free(set->env[i]);
			set->env[i] = ft_strjoin(index, content);
			free(index);
			free(content);
		}
		i++;
	}
}

int	cd_command(t_set *set, char **path)
{
	int	i;

	i = 1;
	if (path[i] == NULL)
	{
		chdir("/home/oszwalbe");
		change_var_of_env(set);
		return (0);
	}
	if (ft_strncmp(path[i], "..", 2) == 0)
	{
		if (chdir("..") != 0)
		{
			perror("chdir() error");
			return (1);
		}
		change_var_of_env(set);
	}
	else
	{
		if (path[i + 1])
		{
			ft_putendl_fd(" too many arguments", 2);
			return (1);
		}
		if (chdir(path[i]) != 0)
		{
			perror("chdir() error");
			return (1);
		}
		change_var_of_env(set);
	}
	return (0);
}
