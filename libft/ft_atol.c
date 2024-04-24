/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:23:06 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/21 16:23:08 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_check(char *num, long *sign)
{
	size_t	i;

	i = 0;
	*sign = 1;
	if (num[i] == '-')
	{
		*sign = -1;
		i++;
	}
	if (ft_strlen(&num[i]) > 19)
		return (1);
	if (ft_strlen(num) == 20 && (ft_strcmp(num, "9223372036854775807") != 0
			|| (ft_strcmp(num, "-9223372036854775808") != 0)))
		return (1);
	return (0);
}

long	ft_atol(char *num)
{
	long	res;
	size_t	i;
	long	sign;

	res = 0;
	i = 0;
	if (ft_check(num, &sign))
		return (0);
	if (num[i] == '-' || num[i] == '+')
			i++;
	while (num[i])
	{
		res = (res * 10) + num[i] - 48;
		i++;
	}
	return (res * sign);
}
