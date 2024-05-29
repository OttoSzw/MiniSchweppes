/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:22:59 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 15:08:14 by oszwalbe         ###   ########.fr       */
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
	if (sig == SIGINT)
	{
		if (g_signal == 2)
		{
			close(STDIN_FILENO);
			write(2, "^C", 2);
			g_signal = 130;
			return ;
		}
		else
		{
			g_signal = 0;
			write(2, "^C\n", 3);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	if (sig == SIGQUIT)
	{
		if (g_signal == 0 || g_signal == 2)
			return ;
		printf("Quit(core dumped)\n");
	}
}

void	signals(void)
{
	struct sigaction	waouh;

	rl_catch_signals = 0;
	sigemptyset(&waouh.sa_mask);
	waouh.sa_flags = 0;
	waouh.sa_handler = handle_sigint;
	sigaction(SIGQUIT, &waouh, NULL);
	sigaction(SIGINT, &waouh, NULL);
}

void	init_struct(t_set *set, char **env)
{
	set->nb_arg = 0;
	set->id = -1;
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
