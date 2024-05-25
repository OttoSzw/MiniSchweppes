/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:24:23 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:24:25 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char **env, int size)
{
	char	**env2;
	int		i;

	i = 0;
	env2 = (char **)ft_calloc(sizeof(char *), (size + 2));
	if (!env2)
	{
		return (free_tab(env), NULL);
	}
	while (env[i])
	{
		env2[i] = ft_strdup(env[i]);
		i++;
	}
	env2[size] = NULL;
	env2[size + 1] = NULL;
	free_tab(env);
	return (env2);
}

int	check_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '=')
		return (0);
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A'
					&& str[i] <= 'Z')))
			return (0);
		i++;
	}
	return (1);
}

void	sort_list_ascii(char **export_env)
{
	int	i;
	int	j;
	int	min_index;

	i = 0;
	while (export_env[i])
	{
		min_index = i;
		j = i + 1;
		while (export_env[j])
		{
			if (ft_strcmp(export_env[j], export_env[min_index]) < 0)
				min_index = j;
			j++;
			swap_strings(&export_env[min_index], &export_env[i]);
		}
		i++;
	}
}

int	success_find(char **env, char *tab2)
{
	char	*str;
	int		i;

	i = 0;
	str = malloc(sizeof(char) * (exportlen(tab2) + 1));
	i = 0;
	while (tab2[i] && tab2[i] != '=')
	{
		str[i] = tab2[i];
		i++;
	}
	str[i] = '\0';
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], str, exportlen(tab2)) == 0)
		{
			free(env[i]);
			env[i] = ft_strdup(tab2);
			return (free(str), 1);
		}
		i++;
	}
	return (free(str), 0);
}

int	export_command(t_set *set, char **c, int size2c)
{
	int	j;
	int	size;

	int (i) = 0;
	j = 0;
	size = tab_calculate(set->env);
	if (!export_alone(set, c))
		return (0);
	i++;
	while (i < size2c)
	{
		if (check_arg(c[i]))
		{
			if (!(success_find(set->env, c[i])))
			{
				set->env = ft_realloc(set->env, size);
				set->env[size] = ft_strdup(c[i]);
				size++;
			}
		}
		else
			return (ft_putendl_fd(" not a valid identifier\n", 2), 1);
		i++;
	}
	return (0);
}
