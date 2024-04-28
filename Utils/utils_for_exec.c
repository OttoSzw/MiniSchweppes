/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:27:03 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/21 17:27:05 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_paths(char **split)
{
	int	i;

	i = 0;
	if (split)
	{
		while (split[i])
		{
			free(split[i]);
			i++;
		}
		free(split);
	}
}

void	error_cmd(void)
{
	ft_putendl_fd("Command not found", 2);
	exit(EXIT_FAILURE);
}

int	error_mess(void)
{
	char	*s;

	s = "Error";
	perror(s);
	return (EXIT_FAILURE);
}

char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*part_paths;
	int		i;

	i = 0;
	while (env[i] && ft_strnstr(env[i], "PATH", 4) == NULL)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_paths = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_paths, cmd);
		free(part_paths);
		if (access(path, F_OK) == 0)
			return (free_paths(paths), path);
		free(path);
		i++;
	}
	return (free_paths(paths), NULL);
}

void	escape(char *path)
{
	free(path);
	error_mess();
}

void	execute_command(char **av, char **env)
{
	char	**cmd;
	char	*path;

	if (!av[1])
	{
		cmd = ft_split(av[0], ' ');
	}
	else
	{
		cmd = av;
	}
	if (!cmd[0])
	{
		free_paths(cmd);
		error_cmd();
	}
	if (access(cmd[0], F_OK) == 0)
	{
		path = ft_strdup(cmd[0]);
		if (!path)
			escape(path);
	}
	else
	{
		path = find_path(cmd[0], env);
		if (!path)
		{
			printf("command not found\n");
			return ;
		}
	}
	if (execve(path, cmd, env) == -1)
		escape(path);
}
