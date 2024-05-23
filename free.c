#include "minishell.h"

extern int	g_signal;

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
	signals();
	set->input = NULL;
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
	set->i = 0;
	set->saved_in = 0;
	set->saved_out = 0;
	set->append = 0;
	set->index = 0;
	set->index2 = 0;
	set->dq = 0;
	set->sq = 0;
	set->return_value = 0;
	set->expand = 0;
	set->flag_pipe = 0;
}

void	free_struct(t_set *set)
{
	int	j;

	j = 0;
	if (set->cmd != NULL)
		free_tab(set->cmd);
	if (set->env != NULL)
		free_tab(set->env);
	if (set->files != NULL)
		free_tab(set->files);
	if (set->input != NULL)
		free(set->input);
	if (set->rdd != 0)
		free(set->rdd);
	j = 0;
	if (set->c != NULL)
	{
		while (set->c[j])
		{
			free_tab(set->c[j]);
			j++;
		}
		free(set->c);
	}
}

void	free_struct2(t_set *set)
{
	if (set->cmd != NULL)
		free_tab(set->cmd);
	if (set->env != NULL)
		free_tab(set->env);
	if (set->files != NULL)
		free_tab(set->files);
	if (set->input != NULL)
		free(set->input);
	if (set->rdd != 0)
		free(set->rdd);
}
