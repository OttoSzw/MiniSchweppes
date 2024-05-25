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

void	replace_var(t_set *set, char *content, int i, int j)
{
	char	*index;

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

void	change_var_of_env(t_set *set)
{
	int		i;
	int		j;
	char	cwd[1024];
	char	*content;
	char	*line;

	i = 0;
	j = 0;
	line = getcwd(cwd, sizeof(cwd));
	if (!line)
		return (ft_putendl_fd("Continue to 'cd ..' :)", 1));
	content = ft_strdup(line);
	if (!content)
	{
		printf("Impossible to move in the directory that's dont exist !\n");
		return ;
	}
	replace_var(set, content, i, j);
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
			return (perror("chdir() error"), 1);
		change_var_of_env(set);
	}
	else
	{
		if (path[i + 1])
			return (ft_putendl_fd(" too many arguments", 2), 1);
		if (chdir(path[i]) != 0)
			return (perror("chdir() error"), 1);
		change_var_of_env(set);
	}
	return (0);
}
