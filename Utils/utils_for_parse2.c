/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_parse2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:19:54 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/27 11:19:56 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	first_size_sq(t_set *set, int *i, int *counter)
{
	(*i)++;
	if (set->input[*i] == '\'')
		(*i)++;
	if (set->input[*i] != '\'')
		(*counter)++;
	while (set->input[*i] && set->input[*i] != '\'')
		(*i)++;
	while (set->input[*i] != '\0' && set->input[*i] != ' ')
		(*i)++;
}

void	first_size_dq(t_set *set, int *i, int *counter)
{
	(*i)++;
	if (set->input[*i] == '\"')
		(*i)++;
	if (set->input[*i] != '\"')
		(*counter)++;
	while (set->input[*i] && set->input[*i] != '\"')
		(*i)++;
	while (set->input[*i] != '\0' && set->input[*i] != ' ')
		(*i)++;
}

void	other_things(t_set *set, int *i, int *counter)
{
	(*counter)++;
	while (set->input[*i] && set->input[*i] != ' '
		&& set->input[*i] != '\0')
	{
		if (set->input[*i] && (set->input[*i] == '\''
				|| set->input[*i] == '\"'))
		{
			(*i)++;
			while (set->input[*i] && set->input[*i] != '\''
				&& set->input[*i] != '\"')
				(*i)++;
		}
		if (set->input[*i])
			(*i)++;
	}
}

int	find_size_parse(t_set *set)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (set->input[i])
	{
		while (set->input[i] == ' ')
			i++;
		if (set->input[i] == '\'')
			first_size_sq(set, &i, &counter);
		else if (set->input[i] == '\"')
			first_size_dq(set, &i, &counter);
		else
			other_things(set, &i, &counter);
		while (set->input[i] && set->input[i] == ' ')
			i++;
	}
	return (counter);
}
