#include "../minishell.h"

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

int	check_redirections(char **av)
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


char	*find_file_out(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i + 1] && ((ft_strcmp(">", cmd[i]) == 0) || (ft_strcmp(">>",
						cmd[i]) == 0)))
		{
			return (cmd[i + 1]);
		}
		i++;
	}
	return (NULL);
}

char	*find_file_in(char **cmd)
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

int	count_nb_files(char **av)
{
	int	i;
	int	counter;

	counter = 0;
	i = 0;
	while (av[i])
	{
		if (ft_strcmp(">", av[i]) == 0 || ft_strcmp(">>", av[i]) == 0)
			counter++;
		i++;
	}
	return (counter);
}

char	*find_file_out2(t_set *set, char **cmd)
{
	int	i;

	i = set->index;
	while (cmd[i])
	{
		if (cmd[i + 1] && ((ft_strcmp(">", cmd[i]) == 0) || (ft_strcmp(">",
						cmd[i]) == 0)))
		{
			set->index = i + 1;
			return (cmd[i + 1]);
		}
		i++;
	}
	set->index = i;
	return (NULL);
}