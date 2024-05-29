/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_exec3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 09:54:30 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 13:21:25 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_no_redir_pipe(t_set *set, char **c)
{
	if (yes_or_no_builtins(set, c) == 1)
	{
		set->return_value = do_builtins(set, c);
		free_struct(set);
		exit(set->return_value);
	}
	else
	{
		close(set->saved_in);
		close(set->saved_out);
		execute_command(set, c, set->env);
	}
}

void	init_redir_pipe(t_set *set, int nb_files, int rd, char **c)
{
	char	*file;
	int		i;

	i = 0;
	set->files = malloc(sizeof(char *) * (nb_files + 1));
	set->rdd = malloc(sizeof(int) * (nb_files + 1));
	if (!set->files)
		return ;
	while (i < nb_files)
	{
		file = find_file_out2(set, c);
		rd = check_redirections(set, c);
		if (file)
		{
			set->files[i] = ft_strdup(file);
			set->rdd[i] = rd;
		}
		i++;
	}
	set->files[i] = NULL;
	set->rdd[i] = -1;
}

void	if_redir_pipe(t_set *set, int nb_files, int rd, char **c)
{
	char	**cmd;

	init_redir_pipe(set, nb_files, rd, c);
	do_redir(set, nb_files, set->j);
	if (yes_or_no_builtins(set, c) == 1)
	{
		set->return_value = do_builtins(set, c);
		free_struct(set);
		exit(set->return_value);
	}
	else
	{
		cmd = copy_tabcmd(set, c);
		if (!cmd)
		{
			close(set->saved_in);
			free_struct(set);
			close(set->saved_out);
			exit(1);
		}
		set->need_to_free = 1;
		close(set->saved_in);
		close(set->saved_out);
		execute_command(set, cmd, set->env);
	}
}

void	command(char ***s, char **c, t_set *set)
{
	int	rd;
	int	nb_files;

	(void)s;
	set->append = 0;
	set->index = 0;
	set->index2 = 0;
	rd = redir_or_not(c);
	nb_files = count_nb_files(c);
	if (rd && (set->dq != 1 && set->sq != 1))
		if_redir_pipe(set, nb_files, rd, c);
	else
		child_no_redir_pipe(set, c);
}
