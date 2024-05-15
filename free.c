#include "minishell.h"

void	free_struct(t_set *set)
{
	free_tab(set->cmd);
	free_tab(set->env);
	free(set->input);
}