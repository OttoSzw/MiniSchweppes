/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:27:31 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 14:43:48 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal;

int	check_grammary(t_set *set, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		pass_the_quote(str, &i);
		if (check_gram_out(str, &i) || check_gram_in(str, &i))
			return (set->return_value = 2, 2);
		if (pass_the_pipe(set, str, &i) == 2)
			return (2);
		if (set->input[i])
			i++;
	}
	return (0);
}

char	*do_here_doc(t_set *set, int nb, char *limiter, char *and)
{
	char	*number;
	char	*file;
	int		fd;

	number = ft_itoa(nb);
	file = ft_strdup(".infile_");
	set->file = ft_strjoin(file, number);
	free(number);
	free(file);
	fd = open(set->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("Chef c est mauvais la\n");
		return (NULL);
	}
	here_doc(set, limiter, and, fd);
	close(fd);
	reset_fd(set);
	return (set->file);
}

void	here_doggy(t_set *set)
{
	int		i;
	int		nb;
	char	*file;

	i = 0;
	nb = 0;
	while (set->cmd[i])
	{
		if (ft_strcmp("<<", set->cmd[i]) == 0)
		{
			i++;
			file = do_here_doc(set, nb, set->cmd[i], set->cmd[i + 1]);
			free(set->cmd[i]);
			set->cmd[i] = ft_strdup(file);
			free(set->file);
			nb++;
		}
		i++;
	}
}

void	main2(t_set *set)
{
	signals();
	init_read(set);
	if (g_signal == 130)
	{
		set->return_value = g_signal;
		g_signal = 0;
	}
	if (set->input == NULL)
	{
		free_tab(set->env);
		printf("exit");
		exit(0);
	}
	set->cmd = NULL;
	if (ft_occurence(set->input) != ft_strlen(set->input))
	{
		add_history(set->input);
		set->i = 0;
		if (check_grammary(set, set->input) == 0)
		{
			set->cmd = parse(set);
			if (set->cmd != NULL)
				here_doggy(set);
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_set	set;

	(void)av;
	if (ac < 1)
		return (1);
	g_signal = 0;
	init_struct(&set, env);
	while (1)
	{
		main2(&set);
		if (set.cmd)
		{
			if (set.expand == 1)
				if (expand(&set) == 0)
					break ;
			set.size_tab = tab_calculate(set.cmd);
			executable(&set);
			free_tab(set.cmd);
		}
		free(set.input);
	}
	free_tab(set.env);
	clear_history();
	return (0);
}
