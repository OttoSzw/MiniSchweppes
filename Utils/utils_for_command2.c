/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_command2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 14:37:57 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 13:26:30 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_redir(t_set *set, int nb_files, int rd)
{
	char	*file;
	int		i;

	i = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	set->files = malloc(sizeof(char *) * (nb_files + 1));
	set->rdd = malloc(sizeof(int) * (nb_files + 1));
	if (!set->files)
		return ;
	while (i < nb_files)
	{
		file = find_file_out2(set, set->cmd);
		rd = check_redirections(set, set->cmd);
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

void	norm_for_outfile(t_set *set, int fd, int i)
{
	if (set->rdd[i] == 4)
		fd = open(set->files[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(set->files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_mess();
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	do_redir(t_set *set, int nb_files, int i)
{
	int	fd;

	i = 0;
	while (i < nb_files)
	{
		if (set->rdd[i] == 1 || set->rdd[i] == 3)
		{
			fd = open(set->files[i], O_RDONLY);
			if (fd == -1)
			{
				free_struct(set);
				close(set->saved_in);
				close(set->saved_out);
				error_mess();
			}
			dup2(fd, STDIN_FILENO);
			if (set->rdd[i] == 3)
				unlink(set->files[i]);
			close(fd);
		}
		else if (set->rdd[i] == 4 || set->rdd[i] == 2)
			norm_for_outfile(set, fd, i);
		i++;
	}
}

void	child_redir(t_set *set, int nb_files, int rd)
{
	char	**cmd;

	init_redir(set, nb_files, rd);
	do_redir(set, nb_files, set->j);
	if (yes_or_no_builtins(set, set->cmd) == 1)
		set->return_value = do_builtins(set, set->cmd);
	else
	{
		cmd = copy_tabcmd(set, set->cmd);
		if (!cmd)
		{
			close(set->saved_in);
			free_struct2(set);
			close(set->saved_out);
			exit(1);
		}
		set->need_to_free = 1;
		close(set->saved_in);
		close(set->saved_out);
		execute_command(set, cmd, set->env);
	}
	close(set->saved_in);
	free_struct2(set);
	close(set->saved_out);
	exit(1);
}

void	if_redir(t_set *set, int nb_files, int rd)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	set->id = fork();
	if (set->id == 0)
		child_redir(set, nb_files, rd);
	reset_fd(set);
	waitpid(set->id, &status, 0);
	if (WIFEXITED(status))
		set->return_value = WEXITSTATUS(status);
}
