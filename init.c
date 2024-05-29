/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:22:59 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:23:01 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

void	init_fd(t_set *set)
{
	set->saved_in = dup(STDIN_FILENO);
	set->saved_out = dup(STDOUT_FILENO);
}

void	reset_fd(t_set *set)
{
	dup2(set->saved_in, STDIN_FILENO);
	dup2(set->saved_out, STDOUT_FILENO);
	close(set->saved_in);
	close(set->saved_out);
}

void	handle_sigint(int sig)
{
	if (g_signal == 2)
	{
		g_signal = 130;
		write(2, "\n", 1);
		return ;
	}
	g_signal = 130;
	(void)sig;
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signals(void)
{
	struct sigaction	waouh;

	signal(SIGQUIT, SIG_IGN);
	ft_bzero(&waouh, sizeof(waouh));
	waouh.sa_handler = handle_sigint;
	sigaction(SIGINT, &waouh, NULL);
}

void	init_struct(t_set *set, char **env)
{
	set->nb_arg = 0;
	set->nb_case = 0;
	set->input = NULL;
	set->stop = 0;
	set->env = copy_of_tab(env);
	set->cmd = NULL;
	set->files = NULL;
	set->c = NULL;
	set->file = NULL;
	set->need_to_free = 0;
	set->saved_fd = 0;
	set->rdd = 0;
	set->return_value = 0;
	set->flag_pipe = 0;
	set->size_tab = 0;
	init_strcut2(set);
}
