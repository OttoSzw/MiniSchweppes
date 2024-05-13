#include "../minishell.h"

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

void	exec_multiple_pipe(char ***c, t_set *set, int size)
{
	int	i;
	int	pipe_fd[2];
	int	id;
	int		status;


	init_fd(set);
	i = 0;
	while (i < size)
	{
		if (pipe(pipe_fd) == -1)
			error_mess();
		id = fork();
		if (id == -1)
			error_mess();
		if (id == 0)
		{
			if (i == 0)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
				close(set->saved_in);
				close(set->saved_out);
				command(c[i], set);
			}
			else if (i == (size - 1))
			{
				close(pipe_fd[1]);
				close(pipe_fd[0]);
				close(set->saved_in);
				close(set->saved_out);
				command(c[i], set);
			}
			else
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
				close(set->saved_in);
				close(set->saved_out);
				command(c[i], set);
			}
		}
		else
		{
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		i++;
	}
	reset_fd(set);
	while (waitpid(id, &status, 0) != -1)
		continue ;
	if (WIFEXITED(status))
		set->return_value = WEXITSTATUS(status);
}

void	parse_for_pipe(t_set* set)
{
	char ***c;
	int	nb_arg;
	// int	i;
	int j;

	// i = 0;
	j = 0;
	c = copy_of_tab_of_tab(set, set->cmd);
	nb_arg = count_cmdpipe(set->cmd);
	// while (c[i])
	// {
	// 	printf("\nCase %d :\n", i);
	// 	j = 0;
	// 	while (c[i][j])
	// 	{
	// 		printf("-> : %s\n", c[i][j]);
	// 		j++;
	// 	}
	// 	i++;
	// }
	exec_multiple_pipe(c, set, nb_arg);
	while (c[j])
	{
		free_tab(c[j]);
		j++;
	}
	free(c);
}