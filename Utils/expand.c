/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:24:58 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:25:02 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	new_cmd(t_set *set, int nb, t_expand *expand)
{
	char	*newcmd;
	char	*tmp;

	if (expand->before && expand->expand)
		tmp = ft_strjoin(expand->before, expand->expand);
	else if (expand->before)
		tmp = ft_strdup(expand->before);
	else
		tmp = ft_strdup(expand->expand);
	if (!tmp)
		return (free_cmd(set, nb), 0);
	if (expand->after)
		newcmd = ft_strjoin(tmp, expand->after);
	else
		newcmd = ft_strdup(tmp);
	free(tmp);
	free_expand(expand);
	if (!newcmd)
		return (0);
	free(set->cmd[nb]);
	set->cmd[nb] = ft_strdup(newcmd);
	free(newcmd);
	if (!set->cmd[nb])
		return (0);
	return (1);
}

int	new_arg(t_set *set, int nb, t_expand *expand)
{
	char	*variable;
	int		i;

	i = 0;
	variable = prep_variable(expand);
	if (!variable)
		return (0);
	while (set->env[i])
	{
		if (ft_strncmp(variable, set->env[i], env_len(set->env[i])) == 0)
		{
			replace_expand(set, i, expand, variable);
			free(variable);
			if (!expand->expand)
				return (0);
			if (new_cmd(set, nb, expand) == 0)
				return (0);
			return (1);
		}
		i++;
	}
	free(expand->expand);
	expand->expand = NULL;
	new_cmd(set, nb, expand);
	return (free(variable), 1);
}

int	expand_arg(t_set *set, int nb)
{
	int			i;
	char		*tmp;
	t_expand	expand;

	init_expand(&expand);
	i = 0;
	while (set->cmd[nb][i] && set->cmd[nb][i] != '$')
		i++;
	expand.before = copy_str(set->cmd[nb], i);
	if (!expand.before)
		return (0);
	tmp = ft_substr(set->cmd[nb], ft_strlen(expand.before),
			ft_strlen(set->cmd[nb]));
	if (!tmp)
		return (0);
	i = 0;
	while (tmp[i] && (tmp[i] == '$' || (tmp[i] >= 'A' && tmp[i] <= 'Z')
			|| (tmp[i] >= 'a' && tmp[i] <= 'z') || tmp[i] == '_'))
		i++;
	if (expand_after(tmp, i, &expand) == 0)
		return (free(tmp), 0);
	free(tmp);
	if (new_arg(set, nb, &expand) == 0)
		return (free_expand(&expand), 0);
	return (1);
}

int	expand_quote(t_set *set, int nb)
{
	int		i;
	char	*tmp;

	tmp = ft_strtrim(set->cmd[nb], "\'");
	if (!tmp)
		return (0);
	free(set->cmd[nb]);
	set->cmd[nb] = ft_substr(tmp, 0, ft_strlen(tmp));
	free(tmp);
	if (!set->cmd[nb])
		return (0);
	i = 0;
	while (set->cmd[nb][i])
	{
		if (set->cmd[nb][i] == '\'')
		{
			if (remove_quote(set, nb) == 0)
				return (0);
		}
		i++;
	}
	return (1);
}

int	expand(t_set *set)
{
	int	i;
	int	j;

	i = 0;
	while (set->cmd[i])
	{
		j = 0;
		while (set->cmd[i][j])
		{
			if (set->cmd[i][j] == '\'' && set->dq != 1)
			{
				if (expand_quote(set, i) == 0)
					return (0);
				break ;
			}
			else if (set->cmd[i][j] == '$')
			{
				if (set->cmd[i][j + 1] == '?')
				{
					if (expand_return(set, i) == 0)
						return (0);
				}
				else if (expand_arg(set, i) == 0)
					return (0);
			}
			if (set->cmd[i][j])
				j++;
		}
		if (set->cmd[i])
			i++;
	}
	return (1);
}
