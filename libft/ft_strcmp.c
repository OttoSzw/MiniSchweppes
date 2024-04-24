/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 00:31:35 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/10 00:31:36 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	int	size1;
	int	size2;

	i = 0;
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	if (size1 != size2)
		return (1);
	while (s1[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}
