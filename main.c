/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:27:31 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/20 23:27:33 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_simple_command(t_set *set)
{
	int	id;

	id = fork();
	if (id == 0)
	{
		execute_command(set->cmd, set->env);
	}
	while (wait(NULL) != -1)
		continue ;
}

int	check_pipe(t_set *set)
{
	int	i;

	i = 0;
	while (set->cmd[i])
	{
		if (ft_strcmp(set->cmd[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}

void	executable(t_set *set)
{
	if (check_pipe(set))
		PipeBendoNaBendo(set, set->env);
	else if (ft_strcmp("echo", set->cmd[0]) == 0)
		echo_command(set->cmd + 1);
	else if (ft_strcmp("cd", set->cmd[0]) == 0)
		cd_command(set->cmd[1]);
	else if (ft_strcmp("pwd", set->cmd[0]) == 0)
		pwd_command(set->cmd);
	else if (ft_strcmp("env", set->cmd[0]) == 0)
		env_command(set->cmd, set->env);
	else if (ft_strcmp("exit", set->cmd[0]) == 0)
		exit_command(set->cmd[1], set->size_tab);
	else
	{
		do_simple_command(set);
	}
}

int	main(int ac, char **av, char **env)
{
	t_set	set;
	int		i;

	(void)av;
	(void)env;
	if (ac < 1)
			return (1);
	i = 0;
	while (1)
	{
		set.input = readline("\1\033[38;5;226m\2M\1\033[38;5;220m\2i\1\033[38;5;214m\2"
								"n\1\033[38;5;208m\2i\1\033[38;5;202m\2S\1\033[38;5;196m\2"
								"c\1\033[38;5;202m\2h\1\033[38;5;208m\2w\1\033[38;5;214m\2"
								"e\1\033[38;5;220m\2p\1\033[38;5;226m\2p\1\033[38;5;220m\2"
								"e\1\033[38;5;214m\2s\1\033[38;5;208m\2 \1\033[38;5;208m\2"
								">\1\033[0m ");
		if (set.input == NULL)
			break ;
		set.cmd = NULL;
		if (ft_occurence(set.input) != ft_strlen(set.input))
		{
			add_history(set.input);
			set.i = 0;
			set.cmd = parse(&set);
		}
		if (set.cmd)
		{
			set.env = copy_of_tab(env);
			set.size_tab = tab_calculate(set.cmd);
			executable(&set);
			free_tab(set.cmd);
			free_tab(set.env);
		}
		free(set.input);
 	}
	clear_history();
	return (0);
}
