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
		int fd;

	if (pipex->yes == 1)
	{
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
		int fd;

	if (pipex->yes == 1)
	{
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

// void	PipeBendoNaBendo(t_set *set, char **env)
// {
// 	t_pipex	pipex;
// 	int	i;

// 	i = 0;
// 	if (ft_strcmp(set->cmd[i], "<<") == 0 || ft_strcmp(set->cmd[i], ">>") == 0)
// 	{
// 		pipex.i = 3;
// 		here_doc(set->cmd[2], set->size_tab);
// 		exec_child(set->size_tab, set->cmd, &pipex, env);
// 	}
// 	else
// 	{
// 		if (access(set->cmd[i + 1], F_OK) != -1)
// 		{
// 			pipex.i = 1;
// 			pipex.yes = 1;
// 		}
// 		else
// 		{
// 			pipex.i = 0;
// 			pipex.yes = 0;
// 		}
// 		exec_child(set->size_tab, set->cmd, &pipex, env);
// 		while (wait(NULL) != -1)
// 			continue ;
// 	}
// }

// int	check_redirections(char **av)
// {
// 	int	i;

// 	i = 0;
// 	while (av[i])
// 	{
// 		if (ft_strcmp("<", av[i]) == 0)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

int	count_pipe(char **av)
{
	int	count;
	int	i;

	i = 1;
	count = 0;
	while (av[i])
	{
		if (ft_strcmp("|", av[i]) == 0)
			count++;
		i++;
	}
	return (count);
}

void	PipeBendoNaBendo(t_set *set, char **env)
{
	pid_t id;
	int fd[2];
	int	i;
	int f;
	int	saved_in;
	int saved_out;
	int	count;

	i = 0;
	count = count_pipe(set->cmd);
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	while (i < (count + 1))
	{
		printf("i : %d\n", i);
		if (pipe(fd) == -1)
			printf("not Good\n");
		id = fork();
		if (id == -1)
			printf("realllyyy not good\n");
		if (id == 0)
		{
			if (i == 0)
			{

				f = open(set->cmd[1], O_RDONLY, 0777);
				if (f == -1)
					error_mess();
				dup2(f, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				execute_command2(set->cmd[2], env);
			}
			if (i == count)
			{
				f = open(set->cmd[6], O_WRONLY | O_CREAT | O_TRUNC, 0777);
				if (f == -1)
					error_mess();
				dup2(f, STDOUT_FILENO);
				printf("CHU LAAA\n");
				close(fd[1]);
				close(fd[0]);
				execute_command2(set->cmd[4], env);
			}
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
		}
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		i++;
	}
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	while (wait(NULL) != -1)
		continue ;
}
