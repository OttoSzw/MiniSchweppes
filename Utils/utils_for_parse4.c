/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_parse4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:58:01 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/28 16:20:01 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] && (str[i + 1] != '\"' && str[i + 1] != ' '))
				return (1);
		}
		i++;
	}
	return (0);
}

void	dq_for_quotes(t_set *set, int *i, int *counter)
{
	check_sq_dq(set);
	if (check_dollar(set->input))
		set->expand = 1;
	(*i)++;
	if (set->input[*i] == '\"')
		(*i)++;
	while (set->input[*i] && set->input[*i] != '\"')
	{
		(*i)++;
		(*counter)++;
		if (set->input[*i] && (set->input[*i] == '\"' && set->input[(*i)
					+ 1] != ' '))
		{
			(*i)++;
			while (set->input[*i])
			{
				(*i)++;
				(*counter)++;
			}
		}
	}
}

void	sq_for_quotes(t_set *set, int *i, int *counter)
{
	if (set->input[(*i) + 1] && set->input[(*i) + 1] == '$')
	{
		(*counter) += 2;
		set->expand = 1;
	}
	(*i)++;
	if (set->input[(*i)] == '\'')
		(*i)++;
	while (set->input[(*i)] && set->input[(*i)] != '\'')
	{
		(*i)++;
		(*counter)++;
		if (set->input[(*i)] && (set->input[(*i)] == '\'' && set->input[(*i)
					+ 1] != ' '))
		{
			(*i)++;
			while (set->input[(*i)] && set->input[(*i)] != ' ')
			{
				(*i)++;
				(*counter)++;
			}
			return ;
		}
	}
}

int	find_size_quotes(t_set *set, int i)
{
	int	counter;

	counter = 0;
	while (set->input[i])
	{
		if (set->input[i] == '\"')
			return (dq_for_quotes(set, &i, &counter), counter);
		else if (set->input[i] == '\'')
		{
			check_sq_dq(set);
			sq_for_quotes(set, &i, &counter);
			return (counter);
		}
		if (set->input[i])
			i++;
	}
	return (counter);
}

void	find_write_exec(int *i, char **cmd)
{
	if (ft_strcmp(cmd[*i], "echo") != 0)
		while (cmd[*i] && ft_strcmp(cmd[*i], "echo") != 0)
			(*i)++;
	(*i)++;
}
