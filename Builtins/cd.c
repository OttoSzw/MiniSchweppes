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

int	cd_command(char *path)
{
	if (path == NULL)
	{
		chdir("/home/oszwalbe");
		return (0);
	}
	if (ft_strncmp(path, "..", 2) == 0)
	{
		if (chdir("..") != 0)
		{
			perror("chdir() error");
			return (1);
		}
	}
	else
	{
		if (chdir(path) != 0)
		{
			perror("chdir() error");
			return (1);
		}
	}
	return (0);
}
