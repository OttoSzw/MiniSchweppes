/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_files2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:30:03 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 13:57:59 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_index(t_set *set, int i)
{
	set->index = i + 1;
}

void	set_index2(t_set *set, int i)
{
	set->index2 = i + 1;
}

int	check_gram_in(char *str, int *i)
{
	if (str[*i] == '<')
	{
		(*i)++;
		if (str[*i] == '<')
			(*i)++;
		while (str[*i] == ' ' || str[*i] == '\'' || str[*i] == '\"')
			(*i)++;
		if (!str[*i] || str[*i] == '>' || str[*i] == '<' || str[*i] == '|')
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			return (1);
		}
	}
	return (0);
}

int	check_gram_out(char *str, int *i)
{
	if (str[*i] == '>')
	{
		(*i)++;
		if (str[*i] == '>')
			(*i)++;
		while (str[*i] == ' ' || str[*i] == '\'' || str[*i] == '\"')
			(*i)++;
		if (!str[*i] || str[*i] == '>' || str[*i] == '<' || str[*i] == '|')
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			return (1);
		}
	}
	return (0);
}
