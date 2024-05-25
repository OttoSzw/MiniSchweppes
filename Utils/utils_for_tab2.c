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
