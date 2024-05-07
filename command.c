#include "minishell.h"

int	check_append(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strcmp(">>", cmd[i]) == 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_redirections(char  **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (ft_strcmp("<", av[i]) == 0)
			return (1);
		else if (ft_strcmp(">", av[i]) == 0)
			return (2);
		else if (ft_strcmp("<<", av[i]) == 0)
			return (3);
		else if (ft_strcmp(">>", av[i]) == 0)
			return (4);
		i++;
	}
	return (0);
}

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
	int count;

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

char **copy_tabcmd(char **cmd)
{
	char **copy;
	int	i;
	int j;

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
	while (cmd[i] && (ft_strcmp(">", cmd[i]) != 0 && ft_strcmp(">>", cmd[i]) != 0))
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

char *find_file_out(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i + 1] && ((ft_strcmp(">", cmd[i]) == 0) || (ft_strcmp(">>", cmd[i]) == 0)))
		{
			return (cmd[i + 1]);
		}
		i++;
	}
	return (NULL);
}

char *find_file_in(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i + 1] && (ft_strcmp("<", cmd[i]) == 0))
		{
			return (cmd[i + 1]);
		}
		i++;
	}
	return (NULL);
}

void	do_simple_command(t_set *set)
{
	int rd;
	int	id;
	int fd;
	char *file_in;
	char *file_out;
	char **cmd;

	init_fd(set);
	set->append = 0;
	set->append = check_append(set->cmd);
	rd = check_redirections(set->cmd);
	file_in = find_file_in(set->cmd);
	file_out = find_file_out(set->cmd);
	if (rd)
	{
		if (rd == 1)
		{
			if (file_in)
			{
				fd = open(file_in, O_RDONLY);
				if (fd == -1)
					error_mess();
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		else if (rd == 3)
			here_doc(set->cmd[1]);
		if (file_out)
		{
			if (set->append == 1)
			{
				fd = open(file_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd == -1)
					error_mess();
			}
			else
			{
				fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
				{
					error_mess();
				}
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		id = fork();
		cmd = copy_tabcmd(set->cmd);
		if (id == 0)
		{
			close(set->saved_in);
			close(set->saved_out);
			execute_command(cmd, set->env);
		}
		free_tab(cmd);
	}
	else
	{
		id = fork();
		if (id == 0)
		{
			close(set->saved_in);
			close(set->saved_out);
			execute_command(set->cmd, set->env);
		}
	}
	reset_fd(set);
	while (wait(NULL) != -1)
		continue ;
}
