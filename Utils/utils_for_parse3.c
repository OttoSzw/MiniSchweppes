/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_parse3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 17:55:45 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/27 17:55:46 by oszwalbe         ###   ########.fr       */
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

char	*find_arg_quoted(t_set *set, int i, int counter, int block)
{
	int		j;
	char	*tempo;

	i = set->i;
	j = 0;
	tempo = malloc(sizeof(char) * (counter + 1));
	while (set->input[i] && i <= (counter + block))
	{
		if (set->input[i] == '\"')
		{
			i++;
			if (set->input[i] == '\"')
				i++;
			while (set->input[i] != '\"')
			{
				if (!set->input[i])
				{
					set->i = i;
					tempo[j] = '\0';
					return (tempo);
				}
				tempo[j++] = set->input[i++];
				if (set->input[i] && (set->input[i] == '\"' && set->input[i
							+ 1] != ' '))
				{
					i++;
					while (set->input[i])
						tempo[j++] = set->input[i++];
					set->i = i;
					tempo[j] = '\0';
					return (tempo);
				}
			}
		}
		else if (set->input[i] == '\'')
		{
			if (set->input[i + 1] && set->input[i + 1] == '$')
			{
				tempo[j++] = set->input[i++];
				while (set->input[i] && set->input[i] != '\'')
				{
					tempo[j++] = set->input[i++];
					if (set->input[i] && set->input[i] == '\'')
						tempo[j++] = set->input[i++];
				}
			}
			else
			{
				i++;
				if (set->input[i] == '\'')
					i++;
				while (set->input[i] && set->input[i] != '\'')
				{
					tempo[j++] = set->input[i++];
					if (set->input[i] && (set->input[i] == '\'' && set->input[i
								+ 1] != ' '))
					{
						i++;
						while (set->input[i] && set->input[i] != ' ')
							tempo[j++] = set->input[i++];
						set->i = i;
						tempo[j] = '\0';
						return (tempo);
					}
				}
				if (set->input[i])
					i++;
			}
		}
		if (set->input[i])
			i++;
	}
	set->i = i;
	tempo[j] = '\0';
	return (tempo);
}
