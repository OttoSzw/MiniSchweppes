/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:25:58 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/21 17:26:01 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_num(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i] > '0' && s[i] < '9')
		i++;
	if (s[i] && (s[i] < '0' || s[i] > '9'))
		return (1);
	return (0);
}

int	check_errors(char *s, long nb)
{
	if (ft_strlen(s) == 19 && ft_strcmp(s, "9223372036854775807") != 0)
	{
		ft_putendl_fd("numeric argument required", 2);
		return (2);
	}
	if (ft_strlen(s) >= 20 && ft_strcmp(s, "-9223372036854775808") != 0)
	{
		ft_putendl_fd("numeric argument required", 2);
		return (2);
	}
	if (is_num(s) == 1)
	{
		ft_putendl_fd(" numeric argument required", 2);
		return (2);
	}
	if (nb < 0 && nb > -255)
		nb -= 256;
	return (nb % 256);
}

int	exit_command(t_set *set, char *s, int size)
{
	long	nb;
	int		error;

	error = 0;
	if (!s)
	{
		free_tab(set->cmd);
		free(set->input);
		free_tab(set->env);
		reset_fd(set);
		printf("exit\n");
		exit(0);
	}
	if (size > 2)
	{
		ft_putendl_fd(" too many arguments", 2);
		return (1);
	}
	printf("exit\n");
	nb = ft_atol(s);
	error = check_errors(s, nb);
	free_tab(set->cmd);
	free(set->input);
	free_tab(set->env);
	reset_fd(set);
	exit(error);
}
