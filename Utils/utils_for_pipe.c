/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:26:36 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 13:12:34 by oszwalbe         ###   ########.fr       */
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

void	child_pipe(int *pipe_fd, t_set *set, char ***c, int i)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	close(set->saved_in);
	close(set->saved_out);
	command(c, c[i], set);
}

void	child_pipe2(int *fd_previous, int *pipe_fd, t_set *set, char ***c)
{
	dup2(*fd_previous, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	close(set->saved_in);
	close(set->saved_out);
	close(*fd_previous);
	command(c, c[set->j], set);
}

void	child_pipe3(int *fd_previous, t_set *set, char ***c, int i)
{
	dup2(*fd_previous, STDIN_FILENO);
	close(set->saved_in);
	close(set->saved_out);
	close(*fd_previous);
	command(c, c[i], set);
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
