/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_pipe2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 16:34:49 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/26 16:34:51 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	end_of_pipe(t_set *set, int size, int i)
{
	int	status;

	reset_fd(set);
	if (i == size)
	{
		waitpid(set->id, &status, 0);
		if (WIFEXITED(status))
			set->return_value = WEXITSTATUS(status);
	}
	while (wait(NULL) != -1)
		continue ;
}

void	init_pipe(t_set *set, int size, int i)
{
	set->j = i;
	if (i != (size - 1))
	{
		if (pipe(set->pipe_fd) == -1)
			error_mess();
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	set->id = fork();
	if (set->id == -1)
		error_mess();
}

void	do_child_pipe(t_set *set, int size, char ***c, int fd_previous)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (set->j == 0)
		child_pipe(set->pipe_fd, set, c, set->j);
	else if (set->j == (size - 1))
		child_pipe3(fd_previous, set, c, set->j);
	else
		child_pipe2(fd_previous, set->pipe_fd, set, c);
}

void	do_parent(t_set *set, int size, int *fd_previous)
{
	if (set->j != 0)
	{
		if (set->j == (size - 1))
			close(*fd_previous);
		else
		{
			close(*fd_previous);
			*fd_previous = set->pipe_fd[0];
			set->saved_fd = *fd_previous;
			close(set->pipe_fd[1]);
		}
	}
	else
	{
		*fd_previous = set->pipe_fd[0];
		close(set->pipe_fd[1]);
	}
}

void	exec_multiple_pipe(char ***c, t_set *set, int size)
{
	int (fd_previous) = 0;
	int (i) = 0;
	init_fd(set);
	while (i < size)
	{
		init_pipe(set, size, i);
		if (set->id == 0)
			do_child_pipe(set, size, c, fd_previous);
		else
		{
			do_parent(set, size, &fd_previous);
		}
		i++;
	}
	end_of_pipe(set, size, i);
}
