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
	if (ft_strcmp("<", cmd[i]) == 0)
	{
		i++;
		if (access(cmd[1], F_OK) == -1)
			error_mess();
		i++;
	}
	while (cmd[i])
	{
		if (ft_strcmp(">", cmd[i]) == 0)
		{
			return (count);
		}
		count++;
		i++;
	}
	return (count);
}

char	**copy_tabcmd(char **cmd)
{
	char	**copy;
	int		i;
	int		j;

	i = 0;
	j = 0;
	copy = (char **)malloc(sizeof(char *) * (count_arg(cmd) + 1));
	if (cmd[i] && ft_strcmp("<", cmd[i]) == 0)
	{
		i++;
		if (access(cmd[1], F_OK) == -1)
			error_mess();
		i++;
	}
	else if (cmd[i] && ft_strcmp("<<", cmd[i]) == 0)
	{
		i++;
		i++;
	}
	while (cmd[i] && (ft_strcmp(">", cmd[i]) != 0 && ft_strcmp(">>",
				cmd[i]) != 0))
	{
		copy[j] = ft_strdup(cmd[i]);
		i++;
		j++;
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

void	do_simple_command(t_set *set)
{
	int		rd;
	int		id;
	int		fd;
	int		nb_files;
	char	*file_in;
	char	*file_out;
	char	**files;
	char	**cmd;
	int		i;
	int		status;

	i = 0;
	init_fd(set);
	set->append = 0;
	set->index = 0;
	set->append = check_append(set->cmd);
	rd = check_redirections(set->cmd);
	nb_files = count_nb_files(set->cmd);
	file_in = find_file_in(set->cmd);
	if (rd && (set->dq != 1 && set->sq != 1))
	{
		id = fork();
		if (id == 0)
		{
			files = malloc(sizeof(char *) * (nb_files + 1));
			if (!files)
				return ;
			// printf("nb files : %d\n", nb_files);
			while (i < nb_files)
			{
				file_out = find_file_out2(set, set->cmd);
				// printf("%s\n", file_out);
				if (file_out)
				{
					files[i] = ft_strdup(file_out);
				}
				i++;
			}
			files[i] = NULL;
			if (rd == 1)
			{
				if (file_in)
				{
					fd = open(file_in, O_RDONLY);
					if (fd == -1)
					{
						free_tab(files);
						free_tab(set->env);
						free_tab(set->cmd);
						close(set->saved_in);
						close(set->saved_out);
						error_mess();
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
				}
			}
			else if (rd == 3)
				here_doc(set, set->cmd[1]);
			i = 0;
			while (i < nb_files)
			{
				if (set->append == 1)
				{
					fd = open(files[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fd == -1)
						error_mess();
				}
				else
				{
					fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd == -1)
					{
						error_mess();
					}
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
				i++;
			}
			free_tab(files);
			if (yes_or_no_builtins(set) == 1)
			{
				do_builtins(set);
			}
			else
			{
				cmd = copy_tabcmd(set->cmd);
				close(set->saved_in);
				close(set->saved_out);
				execute_command(set, cmd, set->env);
				free_tab(cmd);
			}
			free_tab(set->cmd);
			free_tab(set->env);
			close(set->saved_in);
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
		if (yes_or_no_builtins(set) == 1)
		{
			do_builtins(set);
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
