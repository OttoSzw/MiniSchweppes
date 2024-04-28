/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:35:44 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/02/07 15:35:46 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command2(char *av, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(av, ' ');
	if (!cmd[0])
	{
		free_paths(cmd);
		error_cmd();
	}
	if (access(cmd[0], F_OK) == 0)
	{
		path = ft_strdup(cmd[0]);
		if (!path)
			escape(path);
	}
	else
	{
		path = find_path(cmd[0], env);
		if (!path)
		{
			printf("command not found\n");
			return ;
		}
	}
	if (execve(path, cmd, env) == -1)
		escape(path);
}

// void	execute_command(char *av, char **env)
// {
// 	char	**cmd;
// 	char	*path;

// 	cmd = ft_split(av, ' ');
// 	if (!cmd[0])
// 	{
// 		free_paths(cmd);
// 		error_cmd();
// 	}
// 	path = find_path(cmd[0], env);
// 	if (!path)
// 	{
// 		if (access(cmd[0], F_OK) == 0)
// 		{
// 			path = ft_strdup(cmd[0]);
// 			if (!path)
// 				escape(path);
// 		}

// 	}
// 		// escape2(path, cmd);
// 	// else
// 	// {
// 	// 	// path = ft_strdup(cmd[0]);
// 	// 	// if (!path)
// 	// 	// 	escape(path);
// 	// }
// 	if (execve(path, cmd, env) == -1)
// 		escape(path);
// }