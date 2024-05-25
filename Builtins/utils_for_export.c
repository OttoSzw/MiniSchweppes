/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:23:38 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/25 12:23:40 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exportlen(char *tab2)
{
	int	i;

	i = 0;
	while (tab2[i] != '\0' && tab2[i] != '=')
		i++;
	return (i);
}

void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	export_alone(t_set *set, char **c)
{
	char	**export_env;

	if (c[0] && !c[1])
	{
		export_env = copy_of_tab(set->env);
		sort_list_ascii(export_env);
		return (print_tab(export_env), free_tab(export_env), 0);
	}
	return (1);
}
