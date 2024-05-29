/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:57:23 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 09:02:14 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	yes_or_no_builtins(t_set *set, char **c)
{
	int	i;

	i = 0;
	(void)set;
	while (c[i])
	{
		if (ft_strcmp("echo", c[i]) == 0)
			return (1);
		else if (ft_strcmp("cd", c[i]) == 0)
			return (1);
		else if (ft_strcmp("pwd", c[i]) == 0)
			return (1);
		else if (ft_strcmp("env", c[i]) == 0)
			return (1);
		else if (ft_strcmp("exit", c[i]) == 0)
			return (1);
		else if (ft_strcmp("unset", c[i]) == 0)
			return (1);
		else if (ft_strcmp("export", c[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	find_write_size(t_set *set, char **cmd, int *counter, int i)
{
	if (cmd[i])
	{
		if (ft_strcmp(cmd[i], "echo") != 0)
		{
			while (cmd[i] && ft_strcmp(cmd[i], "echo") != 0)
				i++;
		}
		i++;
		if (set->dq != 1)
		{
			while (cmd[i] && ft_strcmp(cmd[i], ">") != 0)
			{
				(*counter)++;
				i++;
			}
			i++;
		}
		else
		{
			while (cmd[i++])
				(*counter)++;
			i++;
		}
	}
}

char	**find_write(t_set *set, char **cmd)
{
	char	**to_copy;

	int (i) = 0;
	int (j) = 0;
	int (counter) = 0;
	find_write_size(set, cmd, &counter, i);
	to_copy = malloc(sizeof(char *) * (counter + 1));
	if (cmd[i])
	{
		find_write_exec(&i, cmd);
		if (set->dq != 1)
		{
			while (cmd[i] && (ft_strcmp(cmd[i], ">") != 0 && ft_strcmp(cmd[i],
						">>") != 0))
				to_copy[j++] = ft_strdup(cmd[i++]);
			i++;
		}
		else
		{
			while (cmd[i])
				to_copy[j++] = ft_strdup(cmd[i++]);
			i++;
		}
	}
	return (to_copy[j] = NULL, to_copy);
}

int	do_builtins(t_set *set, char **c)
{
	char	**to_write;

	int (i) = -1;
	while (c[++i])
	{
		if (ft_strcmp("echo", c[i]) == 0)
		{
			to_write = find_write(set, c);
			set->return_value = echo_command(to_write);
			return (free_tab(to_write), set->return_value);
		}
		else if (ft_strcmp("cd", c[i]) == 0)
			return (cd_command(set, c));
		else if (ft_strcmp("pwd", c[i]) == 0)
			return (pwd_command(c));
		else if (ft_strcmp("env", c[i]) == 0)
			return (env_command(set->env));
		else if (ft_strcmp("exit", c[i]) == 0)
			return (exit_command(set, c[1], tab_calculate(c)));
		else if (ft_strcmp("unset", c[i]) == 0)
			return (unset_command(set, set->env));
		else if (ft_strcmp("export", c[i]) == 0)
			return (export_command(set, c, tab_calculate(c)));
	}
	return (-1);
}

void	executable(t_set *set)
{
	int	j;

	j = 0;
	if (check_pipe(set))
	{
		set->flag_pipe = 1;
		parse_for_pipe(set);
	}
	else
		do_simple_command(set);
}
