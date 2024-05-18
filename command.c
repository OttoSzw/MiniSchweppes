#include "minishell.h"

char	*find_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] && (ft_strcmp(">", cmd[i]) == 0))
		{
			return (cmd[i - 1]);
		}
		else if (cmd[i] && ft_strcmp("<", cmd[i]) == 0)
		{
			return (cmd[i + 2]);
		}
		i++;
	}
	return (NULL);
}

int	count_arg(char **cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		if (ft_strcmp("<", cmd[i]) == 0)
		{
			i += 2;
		}
		else if (ft_strcmp(">", cmd[i]) == 0)
			i += 2;
		else if (ft_strcmp("<<", cmd[i]) == 0)
			i += 2;
		else if (ft_strcmp(">>", cmd[i]) == 0)
			i += 2;
		while (cmd[i] && (ft_strcmp(">", cmd[i]) != 0 && ft_strcmp(">>",
					cmd[i]) != 0 && ft_strcmp("<", cmd[i]) != 0 && ft_strcmp("<<", cmd[i]) != 0))
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**copy_tabcmd(t_set *set, char **cmd)
{
	char	**copy;
	int		i;
	int		j;
	int	size;

	size = count_arg(cmd);
	i = 0;
	j = 0;
	(void)set;
	if (size != 0)
		copy = (char **)malloc(sizeof(char *) * (size + 1));
	else
		return (NULL);
	while (cmd[i])
	{
		if (ft_strcmp("<", cmd[i]) == 0)
		{
			i += 2;
		}
		else if (ft_strcmp(">", cmd[i]) == 0)
			i += 2;
		else if (ft_strcmp("<<", cmd[i]) == 0)
			i += 2;
		else if (ft_strcmp(">>", cmd[i]) == 0)
			i += 2;
		while (cmd[i] && (ft_strcmp(">", cmd[i]) != 0 && ft_strcmp(">>",
					cmd[i]) != 0 && ft_strcmp("<", cmd[i]) != 0 && ft_strcmp("<<", cmd[i]) != 0))
		{
			copy[j] = ft_strdup(cmd[i]);
			i++;
			j++;
		}
	}
	copy[j] = NULL;
	return (copy);
}

void	init_fd(t_set *set)
{
	set->saved_in = dup(STDIN_FILENO);
	set->saved_out = dup(STDOUT_FILENO);
}

void	reset_fd(t_set *set)
{
	dup2(set->saved_in, STDIN_FILENO);
	dup2(set->saved_out, STDOUT_FILENO);
	close(set->saved_in);
	close(set->saved_out);
}

void	print_tab(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("declare -x %s\n", cmd[i]);
		i++;
	}
}

int redir_or_not(char **av)
{
	int	i;
	int	j;

	i = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == '>' || av[i][j] == '<')
				return (1);
			if (av[i][j])
				j++;
		}
		i++;
	}
	return (0);
}

void	do_simple_command(t_set *set)
{
	int		rd;
	int		id;
	int		fd;
	int		nb_files;
	char	*file;
	char	**cmd;
	int		i;
	int		status;

	i = 0;
	init_fd(set);
	set->append = 0;
	set->index = 0;
	set->index2 = 0;
	rd = redir_or_not(set->cmd);
	nb_files = count_nb_files(set->cmd);
	// printf("rd : %d\n", rd);
	// printf("%d\n", set->sq);
	// printf("%d\n", set->dq);
	if (rd && (set->dq != 1 && set->sq != 1))
	{
		id = fork();
		if (id == 0)
		{
			set->files = malloc(sizeof(char *) * (nb_files + 1));
			set->rdd = malloc(sizeof(int) * (nb_files + 1));
			if (!set->files)
				return ;
			// printf("nb files : %d\n", nb_files);
			while (i < nb_files)
			{
				file = find_file_out2(set, set->cmd);
				rd = check_redirections(set, set->cmd);
				// printf("file %d : %s\n", i, file);
				// printf("rd   %d : %d\n", i, rd);
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
				if (set->rdd[i] == 1)
				{
					fd = open(set->files[i], O_RDONLY);
					if (fd == -1)
					{
						free_tab(set->files);
						free(set->rdd);
						free_tab(set->env);
						free_tab(set->cmd);
						close(set->saved_in);
						close(set->saved_out);
						error_mess();
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
				}
				else if (set->rdd[i] == 3)
				{
					here_doc(set, set->files[i], set->cmd[2]);
					if (!set->cmd[2])
					{
						reset_fd(set);
						free_struct(set);
						exit(0);
					}
				}
				else if (set->rdd[i] == 4)
				{
					fd = open(set->files[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fd == -1)
						error_mess();
					dup2(fd, STDOUT_FILENO);
					close(fd);
				}
				else if (set->rdd[i] == 2)
				{
					fd = open(set->files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
					free_struct(set);
					close(set->saved_out);
					exit (1);
				}
				// print_tab(cmd);
				close(set->saved_in);
				close(set->saved_out);
				execute_command(set, cmd, set->env);
				free_tab(cmd);
			}
			close(set->saved_in);
			free_struct(set);
			close(set->saved_out);
			exit (1);
		}
		reset_fd(set);
		waitpid(id, &status, 0);
		if (WIFEXITED(status))
			set->return_value = WEXITSTATUS(status);
	}
	else
	{
		if (yes_or_no_builtins(set, set->cmd) == 1)
		{
			do_builtins(set, set->cmd);
			reset_fd(set);
		}
		else
		{
			id = fork();
			if (id == 0)
			{
				close(set->saved_in);
				close(set->saved_out);
				execute_command(set, set->cmd, set->env);
			}
			reset_fd(set);
			waitpid(id, &status, 0);
			if (WIFEXITED(status))
				set->return_value = WEXITSTATUS(status);
		}
	}
}
