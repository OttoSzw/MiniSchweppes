/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_main2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:08:15 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 10:29:10 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pass_the_quote(char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
	if (str[*i] == '\'')
	{
		(*i)++;
		while (str[*i] && str[*i] != '\'')
			(*i)++;
	}
	if (str[*i] == '\"')
	{
		(*i)++;
		while (str[*i] && str[*i] != '\"')
			(*i)++;
	}
}

int	check_before_pipe(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		pass_the_quote(str, &i);
		if (str[i] == '|')
		{
			i--;
			while (i > 0 && (str[i] == ' ' || str[i] == '\'' || str[i] == '\"'
					|| str[i] == '<' || str[i] == '>'))
				i--;
			if (i == 0)
				return (-1);
			return (1);
		}
		i++;
	}
	return (1);
}

int	pass_the_pipe(t_set *set, char *str, int *i)
{
	if (check_before_pipe(str) == -1)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		set->return_value = 2;
		return (2);
	}
	if (str[*i] == '|')
	{
		(*i)++;
		while (str[*i] == ' ' || str[*i] == '\'' || str[*i] == '\"'
			|| str[*i] == '>' || str[*i] == '<')
			(*i)++;
		if (!str[*i] || str[*i] == '|')
		{
			printf("bash: syntax error near unexpected token `|'\n");
			set->return_value = 2;
			return (2);
		}
	}
	return (0);
}

void	init_read(t_set *set)
{
	set->expand = 0;
	set->flag_pipe = 0;
	set->input = readline(COLOR);
}
