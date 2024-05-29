/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_parse3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 17:55:45 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/28 12:00:00 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dq_for_write(t_set *set, int *i, char **tempo, int *j)
{
	(*i)++;
	if (set->input[*i] == '\"')
		(*i)++;
	while (set->input[*i] != '\"')
	{
		if (!set->input[*i])
		{
			set->i = *i;
			(*tempo)[*j] = '\0';
			set->stop = 1;
			return ;
		}
		(*tempo)[(*j)++] = set->input[(*i)++];
		if (set->input[*i] && (set->input[*i] == '\"' && set->input[(*i)
					+ 1] != ' '))
		{
			(*i)++;
			while (set->input[*i])
				(*tempo)[(*j)++] = set->input[(*i)++];
			set->i = *i;
			(*tempo)[*j] = '\0';
			set->stop = 1;
			return ;
		}
	}
}

void	sq_for_write1(t_set *set, int *i, char **tempo, int *j)
{
	(*tempo)[(*j)++] = set->input[(*i)++];
	while (set->input[*i] && set->input[*i] != '\'')
	{
		(*tempo)[(*j)++] = set->input[(*i)++];
		if (set->input[*i] && set->input[*i] == '\'')
			(*tempo)[(*j)++] = set->input[(*i)++];
	}
}

void	sq_for_write2(t_set *set, int *i, char **tempo, int *j)
{
	(*i)++;
	if (set->input[*i] == '\'')
		(*i)++;
	while (set->input[*i] && set->input[*i] != '\'')
	{
		(*tempo)[(*j)++] = set->input[(*i)++];
		if (set->input[*i] && (set->input[*i] == '\'' && set->input[(*i)
					+ 1] != ' '))
		{
			(*i)++;
			while (set->input[*i] && set->input[*i] != ' ')
				(*tempo)[(*j)++] = set->input[(*i)++];
			set->i = *i;
			(*tempo)[*j] = '\0';
			set->stop = 1;
			return ;
		}
	}
	if (set->input[*i])
		(*i)++;
}

char	*find_arg_quoted(t_set *set, int i, int counter, int block)
{
	char	*tempo;

	int (j) = 0;
	i = set->i;
	tempo = malloc(sizeof(char) * (counter + 1));
	while (set->input[i] && i <= (counter + block))
	{
		if (set->input[i] == '\"' || set->input[i] == '\'')
		{
			if (set->input[i] == '\"')
				dq_for_write(set, &i, &tempo, &j);
			else
			{
				if (set->input[i + 1] && set->input[i + 1] == '$')
					sq_for_write1(set, &i, &tempo, &j);
				else
					sq_for_write2(set, &i, &tempo, &j);
			}
			if (set->stop == 1)
				return (tempo);
		}
		if (set->input[i])
			i++;
	}
	return (set->i = i, tempo[j] = '\0', tempo);
}
