#include "minishell.h"

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
		i++;
	}
	return (0);
}

char	*find_cmd(t_set *set)
{
	int	i;

	i = 0;
	while (set->cmd[i])
	{
		if (set->cmd[i] && (ft_strcmp(">", set->cmd[i]) == 0))
		{
			return (set->cmd[i - 1]);
		}
		else if (set->cmd[i] && ft_strcmp("<", set->cmd[i]) == 0)
		{
			return (set->cmd[i + 2]);
		}
		i++;
	}
	return (NULL);
}

int	count_arg(t_set *set)
{
	int	i;
	int count;

	i = 0;
	count = 0;
	if (ft_strcmp("<", set->cmd[i]) == 0)
	{
		i++;
		if (access(set->cmd[1], F_OK) == -1)
			error_mess();
		i++;
	}
	while (set->cmd[i])
	{
		if (ft_strcmp(">", set->cmd[i]) == 0)
		{
			return (count);
		}
		count++;
		i++;
	}
	return (count);
}

char **copy_tabcmd(t_set *set)
{
	char **copy;
	int	i;
	int j;

	i = 0;
	j = 0;
	copy = (char **)malloc(sizeof(char *) * (count_arg(set) + 1));
	if (set->cmd[i] && ft_strcmp("<", set->cmd[i]) == 0)
	{
		i++;
		if (access(set->cmd[1], F_OK) == -1)
			error_mess();
		i++;
	}
	else if (set->cmd[i] && ft_strcmp("<<", set->cmd[i]) == 0)
	{
		i++;
		i++;
	}
	while (set->cmd[i] && ft_strcmp(">", set->cmd[i]) != 0)
	{
		copy[j] = ft_strdup(set->cmd[i]);
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
		printf("tab[%d] = %s\n", i, cmd[i]);
		i++;
	}
}

char *find_file_out(t_set *set)
{
	int	i;

	i = 0;
	while (set->cmd[i])
	{
		if (set->cmd[i + 1] && (ft_strcmp(">", set->cmd[i]) == 0))
		{
			return (set->cmd[i + 1]);
		}
		i++;
	}
	return (NULL);
}

char *find_file_in(t_set *set)
{
	int	i;

	i = 0;
	while (set->cmd[i])
	{
		if (set->cmd[i + 1] && (ft_strcmp("<", set->cmd[i]) == 0))
		{
			return (set->cmd[i + 1]);
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
	rd = check_redirections(set->cmd);
	file_in = find_file_in(set);
	file_out = find_file_out(set);
	if (rd)
	{
		if (rd == 1)
		{
			if (file_in)
			{
				fd = open(file_in, O_RDONLY, 0777);
				if (fd == -1)
					error_mess();
				dup2(fd, STDIN_FILENO);
			}
		}
		else if (rd == 3)
			here_doc(set->cmd[1]);
		if (file_out)
		{
			fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fd == -1)
				error_mess();
			dup2(fd, STDOUT_FILENO);
		}
		id = fork();
		cmd = copy_tabcmd(set);
		print_tab(cmd);
		if (id == 0)
		{
			execute_command(cmd, set->env);
		}
		free_tab(cmd);
	}
	else
	{
		id = fork();
		if (id == 0)
			execute_command(set->cmd, set->env);
	}
	reset_fd(set);
	while (wait(NULL) != -1)
		continue ;
}
