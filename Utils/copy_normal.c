/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:02:16 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/27 12:02:18 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	count_normal(t_set *set, int *i, int *counter)
{
	while (set->input[*i] != '\0' && set->input[*i] != ' ')
	{
		if (set->input[*i] && (set->input[*i] == '\''
				|| set->input[*i] == '\"'))
		{
			if ((set->input[*i] == '\'' || set->input[*i] == '\"')
				&& (set->input[(*i) + 1] && set->input[(*i) + 1] == '$'))
			{
				set->expand = 1;
				(*counter) += 2;
			}
			(*i)++;
			while (set->input[*i] && (set->input[*i] != '\''
					&& set->input[*i] != '\"'))
				def(counter, i);
		}
		else if (set->input[*i] != '\0' && (set->input[*i] != '\''
				&& set->input[*i] != '\"'))
		{
			if (set->input[*i] == '$' && set->input[(*i) + 1])
				set->expand = 1;
			(*counter)++;
		}
		(*i)++;
	}
}

char	*copy_normal_suite2(t_set *set, char *tempo, int i, int j)
{
	if (set->input[i + 1] && set->input[i + 1] == '$')
	{
		tempo[j++] = set->input[i++];
		while (set->input[i] && set->input[i] != '\'')
		{
			tempo[j++] = set->input[i++];
			if (set->input[i] && set->input[i] == '\'')
				tempo[j++] = set->input[i++];
			if (set->input[i] == ' ')
			{
				set->i = i;
				tempo[j] = '\0';
				return (tempo);
			}
		}
	}
	else
	{
		i++;
		while (set->input[i] && set->input[i] != '\'')
			tempo[j++] = set->input[i++];
	}
	return (NULL);
}

void	copy_normal_suite(t_set *set, char **tempo, int *i, int *j)
{
	if (set->input[(*i) + 1] && set->input[(*i) + 1] == '$')
	{
		(*tempo)[(*j)++] = set->input[(*i)++];
		while (set->input[*i] && set->input[*i] != '\'')
		{
			(*tempo)[(*j)++] = set->input[(*i)++];
			if (set->input[*i] && set->input[*i] == '\'')
				(*tempo)[(*j)++] = set->input[(*i)++];
			if (set->input[(*i)] == ' ')
			{
				set->i = (*i);
				(*tempo)[*j] = '\0';
			}
		}
	}
	else
	{
		(*i)++;
		while (set->input[*i] && set->input[*i] != '\'')
			(*tempo)[(*j)++] = set->input[(*i)++];
	}
}

void	continue_copy(t_set *set, char **tempo, int *i, int *j)
{
	if (set->input[*i] != '\0' && (set->input[*i] != '\''
			&& set->input[*i] != '\"'))
		(*tempo)[(*j)++] = set->input[*i];
	if (set->input[*i] != ' ')
		(*i)++;
}

char	*copy_normal(t_set *set)
{
	char	*tempo;

	int (i) = set->i;
	int (j) = 0;
	int (counter) = 0;
	count_normal(set, &i, &counter);
	tempo = malloc(sizeof(char) * (counter + 1));
	oueoue(set, &i);
	while (set->input[i] && set->input[i] != ' ')
	{
		if (set->input[i] && set->input[i] == '\"')
		{
			i++;
			while (set->input[i] && set->input[i] != '\"')
				tempo[j++] = set->input[i++];
		}
		else if (set->input[i] && set->input[i] == '\'')
		{
			if (copy_normal_suite2(set, tempo, i, j) != NULL)
				return (copy_normal_suite(set, &tempo, &i, &j), tempo);
			copy_normal_suite(set, &tempo, &i, &j);
		}
		continue_copy(set, &tempo, &i, &j);
	}
	return (set->i = i, tempo[j] = '\0', tempo);
}
