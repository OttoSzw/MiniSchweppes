/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:28:14 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/20 23:28:16 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_command(t_set *set, char **path)
{
	int	i;

	i = 1;
	if (path[i] == NULL)
	{
		chdir("/home/oszwalbe");
		return (0);
	}
	if (ft_strncmp(path[i], "..", 2) == 0)
	{
		if (chdir("..") != 0)
		{
			perror("chdir() error");
			return (1);
		}
	}
	else
	{
		if (path[i + 1])
		{
			ft_putendl_fd(" too many arguments", 2);
			return (1);
		}
		if (set->expand == 1)
		{
			ft_putendl_fd(" too many arguments", 2);
			return (1);
		}
		if (chdir(path[i]) != 0)
		{
			perror("chdir() error");
			return (1);
		}
	}
	return (0);
}
