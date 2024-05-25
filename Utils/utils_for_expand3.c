/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_expand3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:49:05 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/25 17:49:07 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_norm(t_set *set, int i, int j)
{
	if (set->cmd[i][j + 1] == '?')
	{
		if (expand_return(set, i) == 0)
			return (0);
	}
	else if (expand_arg(set, i) == 0)
		return (0);
	return (1);
}