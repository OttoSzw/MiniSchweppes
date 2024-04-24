/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:51:33 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/02/12 19:51:35 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child(char *av, char *av2, int *pipe_fd, char **env, t_pipex *pipex)
{
	if (pipex->yes == 1)
	{
		int	fd;
		fd = open(av2, O_RDONLY, 0777);
		if (fd == -1)
			error_mess();
		dup2(fd, STDIN_FILENO);
	}
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execute_command2(av, env);
}

void	child2(char *av, int *pipe_fd, char **env)
{
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execute_command2(av, env);
}

void	child3(char *av, char *av2, int *pipe_fd, char **env, t_pipex *pipex)
{
	if (pipex->yes == 1)
	{
		int	fd;
		fd = open(av2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
			error_mess();
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execute_command2(av, env);
}

void	PipeBendoNaBendo(t_set *set, char **env)
{
	t_pipex	pipex;
	int	i;

	i = 0;
	if (ft_strcmp(set->cmd[i], "<<") == 0 || ft_strcmp(set->cmd[i], ">>") == 0)
	{
		pipex.i = 3;
		here_doc(set->cmd[2], set->size_tab);
		exec_child(set->size_tab, set->cmd, &pipex, env);
	}
	else
	{
		if (access(set->cmd[i + 1], F_OK) != -1)
		{
			pipex.i = 1;
			pipex.yes = 1;
		}
		else
		{
			pipex.i = 0;
			pipex.yes = 0;
		}
		exec_child(set->size_tab, set->cmd, &pipex, env);
		while (wait(NULL) != -1)
			continue ;
	}
}
