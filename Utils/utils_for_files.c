/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:25:54 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/24 10:25:55 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	check_redirections(t_set *set, char **av)
{
	int	i;
	int	j;

	i = set->index2;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == '>' && av[i][j + 1] && av[i][j + 1] != '>')
			{
				set->index2 = i + 1;
				return (2);
			}
			else if (av[i][j] == '>' && !av[i][j + 1])
			{
				set->index2 = i + 1;
				return (2);
			}
			else if (av[i][j] == '>' && av[i][j + 2] && av[i][j + 1] == '>')
			{
				set->index2 = i + 1;
				return (4);
			}
			else if (av[i][j] == '>' && !av[i][j + 2] && av[i][j + 1] == '>')
			{
				set->index2 = i + 1;
				return (4);
			}
			else if (av[i][j] == '<' && av[i][j + 1] && av[i][j + 1] != '<')
			{
				set->index2 = i + 1;
				return (1);
			}
			else if (av[i][j] == '<' && !av[i][j + 1])
			{
				set->index2 = i + 1;
				return (1);
			}
			else if (av[i][j] == '<' && av[i][j + 2] && av[i][j + 1] == '<')
			{
				set->index2 = i + 1;
				return (3);
			}
			else if (av[i][j] == '<' && !av[i][j + 2] && av[i][j + 1] == '<')
			{
				set->index2 = i + 1;
				return (3);
			}
			j++;
		}
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

char	*find_file_in2(t_set *set, char **cmd)
{
	int	i;
	int	j;

	i = set->index;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] == '<' && cmd[i][j + 1] && cmd[i][j + 1] != '<')
			{
				set->index = i + 1;
				return (&cmd[i][j + 1]);
			}
			else if (cmd[i][j] == '<' && !cmd[i][j + 1])
			{
				set->index = i + 1;
				return (cmd[i + 1]);
			}
			else if (cmd[i][j] == '<' && cmd[i][j + 2] && cmd[i][j + 1] == '<')
			{
				set->index = i + 1;
				return (&cmd[i][j + 1]);
			}
			else if (cmd[i][j] == '<' && !cmd[i][j + 2] && cmd[i][j + 1] == '<')
			{
				set->index = i + 1;
				return (cmd[i + 1]);
			}
			j++;
		}
		if (cmd[i])
			i++;
	}
	return (NULL);
}

int	count_nb_files(char **av)
{
	int	i;
	int	j;
	int	counter;

	counter = 0;
	i = 0;
	while (av[i])
	{
		j = 0;
		if (av[i][j] == '>' && !av[i][j + 1])
			counter++;
		else if (av[i][j] == '<' && !av[i][j + 1])
			counter++;
		while (av[i][j])
		{
			if (av[i][j] == '>' && av[i][j + 1] && (av[i][j + 1] != '>' && av[i][j - 1] != '>'))
				counter++;
			else if (av[i][j] == '>' && av[i][j + 1] && av[i][j + 1] == '>')
				counter++;
			else if (av[i][j] == '<' && av[i][j + 1] && (av[i][j + 1] != '<' && av[i][j - 1] != '<'))
				counter++;
			else if (av[i][j] == '<' && av[i][j + 1] && av[i][j + 1] == '<')
				counter++;
			if (av[i][j])
				j++;
		}
		i++;
	}
	return (counter);
}

char	*find_file_out2(t_set *set, char **cmd)
{
	int	i;
	int j;

	i = set->index;
	if (!cmd)
		return (NULL);
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] == '>' && cmd[i][j + 1] && cmd[i][j + 1] != '>')
			{
				set->index = i + 1;
				return (&cmd[i][j + 1]);
			}
			else if (cmd[i][j] == '>' && !cmd[i][j + 1])
			{
				set->index = i + 1;
				return (cmd[i + 1]);
			}
			else if (cmd[i][j] == '>' && cmd[i][j + 2] && cmd[i][j + 1] == '>')
			{
				set->index = i + 1;
				return (&cmd[i][j + 1]);
			}
			else if (cmd[i][j] == '>' && !cmd[i][j + 2] && cmd[i][j + 1] == '>')
			{
				set->index = i + 1;
				return (cmd[i + 1]);
			}
			else if (cmd[i][j] == '<' && cmd[i][j + 1] && cmd[i][j + 1] != '<')
			{
				set->index = i + 1;
				return (&cmd[i][j + 1]);
			}
			else if (cmd[i][j] == '<' && !cmd[i][j + 1])
			{
				set->index = i + 1;
				return (cmd[i + 1]);
			}
			else if (cmd[i][j] == '<' && cmd[i][j + 2] && cmd[i][j + 1] == '<')
			{
				set->index = i + 1;
				return (&cmd[i][j + 1]);
			}
			else if (cmd[i][j] == '<' && !cmd[i][j + 2] && cmd[i][j + 1] == '<')
			{
				set->index = i + 1;
				return (cmd[i + 1]);
			}
			j++;
		}
		if (cmd[i])
			i++;
	}
	return (NULL);
}
