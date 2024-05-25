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

char	*copy_char(t_set *set, int size2, int size, char *tempo)
{
	char	*copy;
	int		k;
	int		j;
	int		i;
	int		nb;

	i = 0;
	j = 0;
	k = 0;
	nb = set->j;
	copy = malloc(sizeof(char) * (size2 + (size - 1)));
	while (set->cmd[nb][i] != '$')
		copy[j++] = set->cmd[nb][i++];
	while (k < size)
		copy[j++] = tempo[k++];
	i++;
	while (set->cmd[nb][i] && set->cmd[nb][i + 1])
	{
		copy[j] = set->cmd[nb][i + 1];
		j++;
		i++;
	}
	copy[j] = '\0';
	return (copy);
}
