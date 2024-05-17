#include "minishell.h"

void	init_struct(t_set *set, char **env)
{
	set->input = NULL;
	set->env = copy_of_tab(env);
	set->cmd = NULL;
	set->files = NULL;
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