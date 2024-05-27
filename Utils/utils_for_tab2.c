/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_tab2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:58:42 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/25 19:58:43 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_cmdpipe(char **av)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (av[i])
	{
		if (ft_strcmp("|", av[i]) == 0)
			counter++;
		i++;
	}
	counter++;
	return (counter);
}

void	init_strcut2(t_set *set)
{
	set->i = 0;
	set->j = 0;
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

void	oueoue(t_set *set, int *i)
{
	*i = set->i;
	while (set->input[*i] == ' ')
		(*i)++;
}

void	def(int *counter, int *i)
{
	(*counter)++;
	(*i)++;
}

void	check_sq_dq(t_set *set)
{
	int (i) = 0;
	while (set->input[i])
	{
		if (set->input[i] == '\"')
		{
			i++;
			while (set->input[i] != '\"')
			{
				if (set->input[i] == '>' || set->input[i] == '<')
					set->dq = 1;
				i++;
			}
		}
		else if (set->input[i] == '\'')
		{
			i++;
			while (set->input[i] != '\'')
			{
				if (set->input[i] == '>' || set->input[i] == '<')
					set->sq = 1;
				i++;
			}
		}
		i++;
	}
}
