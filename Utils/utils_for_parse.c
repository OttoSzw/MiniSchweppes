/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:26:29 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:26:31 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*copy_quotes(t_set *set)
{
	int		i;
	int		block;
	int		counter;
	char	*tempo;

	i = set->i;
	counter = find_size_quotes(set, i);
	i = set->i;
	block = i;
	tempo = find_arg_quoted(set, i, counter, block);
	return (tempo);
}

int	check_quotes(t_set *set)
{
	int	i;

	i = 0;
	set->dq = -1;
	set->sq = -1;
	while (set->input[i])
	{
		if (set->input[i] == '\"' && set->sq == -1)
			set->dq *= -1;
		else if (set->input[i] == '\'' && set->dq == -1)
			set->sq *= -1;
		i++;
	}
	if (set->dq == -1 && set->sq == -1)
		return (1);
	return (0);
}

void	parse2(t_set *set, int *i, int *g, char ***split)
{
	(*i) = set->i;
	while (set->input[*i] == ' ')
		(*i)++;
	set->i = (*i);
	if (set->input[*i] == '\'' || set->input[*i] == '\"')
		(*split)[(*g)++] = copy_quotes(set);
	else
		(*split)[(*g)++] = copy_normal(set);
	(*i)++;
}

char	**parse(t_set *set)
{
	char	**split;

	int (g) = 0;
	int (i) = 0;
	int (counter) = 0;
	if (check_quotes(set) == 0)
		return (ft_putendl_fd("Quotes not close !", 2), NULL);
	counter = find_size_parse(set);
	if (counter == 0)
		return (NULL);
	split = ft_calloc(sizeof(char *), (counter + 1));
	set->i = 0;
	i = set->i;
	while (set->input[i] == ' ')
		i++;
	set->i = i;
	while (set->input[i] && g < counter)
		parse2(set, &i, &g, &split);
	split[g] = NULL;
	return (split);
}

size_t	ft_occurence(char *s)
{
	int		i;
	size_t	compteur;

	i = 0;
	compteur = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		if ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ' || s[i] == '\t')
			compteur++;
		i++;
	}
	return (compteur);
}
