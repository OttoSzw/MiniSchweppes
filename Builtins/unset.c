/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:24:36 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:24:39 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_command(t_set *set, char **env)
{
	int	i;

	int (j) = 1;
	if (!set->cmd[j])
		return (0);
	while (j < tab_calculate(set->cmd))
	{
		i = 0;
		while (env[i])
		{
			if (ft_strncmp(set->cmd[j], env[i], ft_strlen(set->cmd[j])) == 0)
			{
				free(env[i]);
				while (env[i + 1])
				{
					env[i] = env[i + 1];
					i++;
				}
				env[i] = NULL;
			}
			i++;
		}
		j++;
	}
	return (0);
}
