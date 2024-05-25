/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:22:31 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:22:52 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_redir(t_set *set, int nb_files, int rd)
{
	int		i;
	int		status;
	char	*file;
	int		fd;
	char	**cmd;

	i = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	set->id = fork();
	if (set->id == 0)
	{
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
			else if (set->rdd[i] == 4)
			{
				fd = open(set->files[i], O_WRONLY | O_CREAT | O_APPEND,
						0644);
				if (fd == -1)
					error_mess();
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else if (set->rdd[i] == 2)
			{
				fd = open(set->files[i], O_WRONLY | O_CREAT | O_TRUNC,
						0644);
				if (fd == -1)
					error_mess();
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			i++;
		}
		if (yes_or_no_builtins(set, set->cmd) == 1)
		{
			do_builtins(set, set->cmd);
		}
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
	reset_fd(set);
	waitpid(set->id, &status, 0);
	if (WIFEXITED(status))
		set->return_value = WEXITSTATUS(status);
}

void	child_no_redir(t_set *set)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(set->saved_in);
	close(set->saved_out);
	execute_command(set, set->cmd, set->env);
}

void	no_redir(t_set *set)
{
	int	id;
	int	status;

	if (yes_or_no_builtins(set, set->cmd) == 1)
	{
		do_builtins(set, set->cmd);
		reset_fd(set);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		id = fork();
		if (id == 0)
			child_no_redir(set);
		reset_fd(set);
		waitpid(id, &status, 0);
		if (WIFSIGNALED(status))
		{
			set->return_value = WTERMSIG(status);
			set->return_value += 128;
		}
		else if (WIFEXITED(status))
			set->return_value = WEXITSTATUS(status);
	}
}

void	do_simple_command(t_set *set)
{
	int		rd;
	int		nb_files;

	init_fd(set);
	set->append = 0;
	set->index = 0;
	set->index2 = 0;
	rd = redir_or_not(set->cmd);
	nb_files = count_nb_files(set->cmd);
	if (rd && (set->dq != 1 && set->sq != 1))
	{
		if_redir(set, nb_files, rd);
	}
	else
		no_redir(set);
}
