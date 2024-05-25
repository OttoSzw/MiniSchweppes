/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:26:36 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:26:38 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_pipe(t_set *set)
{
	int	i;

	i = 0;
	while (set->cmd[i])
	{
		if (ft_strcmp(set->cmd[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}

void	exec_multiple_pipe(char ***c, t_set *set, int size)
{
	int	i;
	int	j;
	int	pipe_fd[2];
	int	id;
	int	status;
	int	fd_previous;

	init_fd(set);
	i = 0;
	j = 0;
	fd_previous = 0;
	while (i < size)
	{
		if (i != (size - 1))
		{
			if (pipe(pipe_fd) == -1)
				error_mess();
		}
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		id = fork();
		if (id == -1)
			error_mess();
		if (id == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (i == 0)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
				close(set->saved_in);
				close(set->saved_out);
				command(c, c[i], set);
			}
			else if (i == (size - 1))
			{
				dup2(fd_previous, STDIN_FILENO);
				close(set->saved_in);
				close(set->saved_out);
				close(fd_previous);
				command(c, c[i], set);
			}
			else
			{
				dup2(fd_previous, STDIN_FILENO);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
				close(set->saved_in);
				close(set->saved_out);
				close(fd_previous);
				command(c, c[i], set);
			}
		}
		else
		{
			if (i != 0)
			{
				if (i == (size - 1))
					close(fd_previous);
				else
				{
					close(fd_previous);
					fd_previous = pipe_fd[0];
					set->saved_fd = fd_previous;
					close(pipe_fd[1]);
				}
			}
			else
			{
				fd_previous = pipe_fd[0];
				close(pipe_fd[1]);
			}
		}
		i++;
	}
	reset_fd(set);
	if (i == size)
	{
		waitpid(id, &status, 0);
		if (WIFEXITED(status))
			set->return_value = WEXITSTATUS(status);
	}
	while (wait(NULL) != -1)
		continue ;
}

void	parse_for_pipe(t_set *set)
{
	int	nb_arg;
	int	i;
	int	j;

	i = 0;
	j = 0;
	set->c = copy_of_tab_of_tab(set, set->cmd);
	nb_arg = count_cmdpipe(set->cmd);
	exec_multiple_pipe(set->c, set, nb_arg);
	j = 0;
	while (set->c[j])
	{
		free_tab(set->c[j]);
		j++;
	}
	free(set->c);
}
