/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_expand2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:25:19 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:25:21 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_after(char *tmp, int i, t_expand *expand)
{
	expand->expand = copy_str(tmp, i);
	if (!expand->expand)
		return (0);
	if (tmp[i])
	{
		expand->after = ft_substr(tmp, i, (ft_strlen(tmp)
					- ft_strlen(expand->expand)));
		if (!expand->after)
			return (0);
	}
	return (1);
}

void	free_cmd(t_set *set, int nb)
{
	free(set->cmd[nb]);
	set->cmd[nb] = NULL;
}

void	replace_expand(t_set *set, int i, t_expand *expand, char *variable)
{
	free(expand->expand);
	expand->expand = ft_substr(set->env[i], ft_strlen(variable),
			(ft_strlen(set->env[i]) - ft_strlen(variable)));
}

char	*prep_variable(t_expand *expand)
{
	char	*variable;
	char	*tmp;

	tmp = ft_strtrim(expand->expand, "/$");
	if (!tmp)
		return (NULL);
	variable = ft_strjoin(tmp, "=");
	free(tmp);
	if (!variable)
		return (NULL);
	return (variable);
}

int	expand_return(t_set *set, int nb)
{
	int		size;
	int		size2;
	char	*tempo;
	char	*copy;

	tempo = ft_itoa(set->return_value);
	size = ft_strlen(tempo);
	size2 = ft_strlen(set->cmd[nb]);
	if (size)
	{
		set->j = nb;
		copy = copy_char(set, size2, size, tempo);
		free(set->cmd[nb]);
		set->cmd[nb] = ft_strdup(copy);
	}
	if (!set->cmd[nb])
		return (0);
	free(copy);
	free(tempo);
	return (1);
}
