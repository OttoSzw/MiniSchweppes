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

void	here_doc(t_set *set, char *limiter, char *av2, int file)
{
	pid_t	reader;
	int		fd[2];
	char	*line;

	line = NULL;
	if (pipe(fd) == -1)
		error_mess();
	init_fd(set);
	reader = fork();
	dup2(set->saved_in, 0);
	if (reader == 0)
	{
		close(fd[0]);
		line = readline(">");
		while (line != NULL)
		{
			if (ft_strcmp(line, limiter) == 0)
			{
				free(line);
				free(set->file);
				reset_fd(set);
				free_struct2(set);
				close(fd[1]);
				close(file);
				exit(EXIT_SUCCESS);
			}
			if (av2 && ft_strcmp("|", av2) != 0)
				ft_putendl_fd(line, file);
			free(line);
			line = readline(">");
		}
		free(set->file);
		reset_fd(set);
		free_struct(set);
		close(file);
		close(fd[1]);
		exit(1);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		wait(NULL);
	}
}

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
	// char	*s;
	// s = "Error";
	// printf("1\n");
	perror(NULL);
	exit(EXIT_FAILURE);
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

void	execute_command(t_set *set, char **av, char **env)
{
	char	**cmd;
	char	*path;

	if (!av[1])
		cmd = ft_split(av[0], ' ');
	else
		cmd = av;
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
			ft_putstr_fd("bash : ", 2);
			ft_putstr_fd(av[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			if (set->need_to_free == 1)
			{
				free_tab(av);
				free_tab(cmd);
				free_struct(set);
				exit(127);
			}
			if (!av[1])
				free_tab(cmd);
			free_struct(set);
			exit(127);
		}
	}
	if (execve(path, cmd, env) == -1)
	{
		free_struct(set);
		free_tab(cmd);
		free_tab(av);
		escape(path);
	}
}
