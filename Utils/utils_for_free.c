/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 11:37:23 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/25 11:37:25 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	free_tab(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

void	free_paths(char **split)
{
	int	i;

	i = 0;
	if (split)
	{
		while (split[i])
		{
			free(split[i]);
			i++;
		}
		free(split);
	}
}
