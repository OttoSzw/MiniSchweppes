/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:22:31 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:22:52 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_no_redir(t_set *set)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(set->saved_in);
	close(set->saved_out);
	execute_command(set, set->cmd, set->env);
}

void	no_redir(t_set *set)
{
	int	id;
	int	status;

	if (yes_or_no_builtins(set, set->cmd) == 1)
	{
		set->return_value = do_builtins(set, set->cmd);
		reset_fd(set);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		id = fork();
		if (id == 0)
			child_no_redir(set);
		reset_fd(set);
		waitpid(id, &status, 0);
		if (WIFSIGNALED(status))
		{
			set->return_value = WTERMSIG(status);
			set->return_value += 128;
		}
		else if (WIFEXITED(status))
			set->return_value = WEXITSTATUS(status);
	}
}

void	do_simple_command(t_set *set)
{
	int	rd;
	int	nb_files;

	init_fd(set);
	set->append = 0;
	set->index = 0;
	set->index2 = 0;
	rd = redir_or_not(set->cmd);
	nb_files = count_nb_files(set->cmd);
	if (rd && (set->dq != 1 && set->sq != 1))
	{
		if_redir(set, nb_files, rd);
	}
	else
		no_redir(set);
}
