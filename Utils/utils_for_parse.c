#include "../minishell.h"

char	*copy_normal(t_set *set)
{
	int		i;
	int		j;
	int		counter;
	char	*tempo;

	i = set->i;
	// printf("ouen est i : %d\n", set->i);
	j = 0;
	counter = 0;
	while (set->input[i] != '\0' && set->input[i] != ' ')
	{
		if (set->input[i] && (set->input[i] == '\'' || set->input[i] == '\"'))
		{
			if (set->input[i] == '\'' && (set->input[i + 1] && set->input[i
				+ 1] == '$'))
			{
				set->expand = 1;
				counter += 2;
			}
			i++;
			while (set->input[i] && (set->input[i] != '\''
					&& set->input[i] != '\"'))
			{
				counter++;
				i++;
			}
		}
		else if (set->input[i] != '\0' && (set->input[i] != '\''
				&& set->input[i] != '\"'))
		{
			if (set->input[i] == '$' && set->input[i + 1])
				set->expand = 1;
			counter++;
		}
		i++;
	}
	// printf("la taille est de %d\n\n", counter);
	tempo = malloc(sizeof(char) * (counter + 1));
	if (!tempo)
	{
		return (NULL);
	}
	i = set->i;
	while (set->input[i] == ' ')
		i++;
	while (set->input[i] && set->input[i] != ' ')
	{
		if (set->input[i] && set->input[i] == '\"')
		{
			i++;
			while (set->input[i] && set->input[i] != '\"')
				tempo[j++] = set->input[i++];
		}
		else if (set->input[i] && set->input[i] == '\'')
		{
			if (set->input[i + 1] && set->input[i + 1] == '$')
			{
				tempo[j] = set->input[i];
				i++;
				j++;
				while (set->input[i] && set->input[i] != '\'')
				{
					tempo[j] = set->input[i];
					i++;
					j++;
					if (set->input[i] && set->input[i] == '\'')
					{
						tempo[j] = set->input[i];
						j++;
						i++;
					}
					if (set->input[i] == ' ')
					{
						set->i = i;
						tempo[j] = '\0';
						return (tempo);
					}
				}
			}
			else
			{
				i++;
				while (set->input[i] && set->input[i] != '\'')
				{
					tempo[j] = set->input[i];
					i++;
					j++;
					if (set->input[i] && (set->input[i] == '\'' && set->input[i
							+ 1] != ' '))
					{
						i++;
						if (set->input[i] == ' ')
						{
							set->i = i;
							tempo[j] = '\0';
							return (tempo);
						}
					}
				}
				if (set->input[i])
					i++;
			}
		}
		else if (set->input[i] != '\0' && (set->input[i] != '\''
				&& set->input[i] != '\"'))
			tempo[j++] = set->input[i];
		if (set->input[i] != ' ')
			i++;
	}
	set->i = i;
	tempo[j] = '\0';
	// printf("tempo = %s\n", tempo);
	return (tempo);
}

int	check_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] && (str[i + 1] != '\"' && str[i + 1] != ' '))
				return (1);
		}
		i++;
	}
	return (0);
}

void	check_sq_dq(t_set *set)
{
	int	i;
	
	i = 0;
	while (set->input[i])
	{
		if (set->input[i] == '\"')
		{
			i++;
			while (set->input[i] != '\"')
			{
				if (set->input[i] == '>' || set->input[i] == '<')
					set->dq = 1;
				i++;
			}
		}
		else if (set->input[i] == '\'')
		{
			i++;
			while (set->input[i] != '\'')
			{
				if (set->input[i] == '>' || set->input[i] == '<')
					set->sq = 1;
				i++;
			}
		}
		i++;
	}
}

int	find_size_quotes(t_set *set, int i)
{
	int	counter;

	counter = 0;
	while (set->input[i])
	{
		if (set->input[i] == '\"')
		{
			check_sq_dq(set);
			if (check_dollar(set->input))
				set->expand = 1;
			i++;
			while (set->input[i] && set->input[i] != '\"')
			{
				i++;
				counter++;
				if (set->input[i] && (set->input[i] == '\"' && set->input[i
						+ 1] != ' '))
				{
					i++;
					while (set->input[i])
					{
						i++;
						counter++;
					}
					return (counter);
				}
			}
			return (counter);
		}
		else if (set->input[i] == '\'')
		{
			check_sq_dq(set);
			if (set->input[i + 1] && set->input[i + 1] == '$')
			{
				counter += 2;
				set->expand = 1;
			}
			i++;
			while (set->input[i] && set->input[i] != '\'')
			{
				i++;
				counter++;
				if (set->input[i] && (set->input[i] == '\'' && set->input[i
						+ 1] != ' '))
				{
					i++;
					while (set->input[i])
					{
						i++;
						counter++;
					}
					return (counter);
				}
			}
			return (counter);
		}
		if (set->input[i])
			i++;
	}
	printf("%d\n", counter);
	return (counter);
}

char *find_arg_quoted(t_set *set, int i, int counter, int block)
{
	int		j;
	char	*tempo;

	i = set->i;
	j = 0;
	tempo = malloc(sizeof(char) * (counter + 1));
	while (set->input[i] && i <= (counter + block))
	{
		if (set->input[i] == '\"')
		{
			i++;
			while (set->input[i] != '\"')
			{
				tempo[j] = set->input[i];
				i++;
				j++;
				if (set->input[i] && (set->input[i] == '\"' && set->input[i
						+ 1] != ' '))
				{
					i++;
					while (set->input[i])
					{
						tempo[j] = set->input[i];
						i++;
						j++;
					}
					set->i = i;
					tempo[j] = '\0';
					return (tempo);
				}
			}
		}
		else if (set->input[i] == '\'')
		{
			if (set->input[i + 1] && set->input[i + 1] == '$')
			{
				tempo[j] = set->input[i];
				i++;
				j++;
				while (set->input[i] && set->input[i] != '\'')
				{
					tempo[j] = set->input[i];
					i++;
					j++;
					if (set->input[i] && set->input[i] == '\'')
					{
						tempo[j] = set->input[i];
						j++;
						i++;
					}
				}
			}
			else
			{
				i++;
				while (set->input[i] && set->input[i] != '\'')
				{
					tempo[j] = set->input[i];
					i++;
					j++;
					if (set->input[i] && (set->input[i] == '\'' && set->input[i
							+ 1] != ' '))
					{
						i++;
						while (set->input[i])
						{
							tempo[j] = set->input[i];
							i++;
							j++;
						}
						set->i = i;
						tempo[j] = '\0';
						return (tempo);
					}
				}
				if (set->input[i])
					i++;
			}
		}
		if (set->input[i])
			i++;
	}
	set->i = i;
	tempo[j] = '\0';
	return (tempo);
}

char	*copy_quotes(t_set *set)
{
	int		i;
	int		block;
	int		counter;
	char	*tempo;

	i = set->i;
	counter = find_size_quotes(set, i);
	i = set->i;
	block = i;
	tempo = find_arg_quoted(set, i, counter, block);
	// printf("tempo = %s\n", tempo);
	return (tempo);
}

int	check_quotes(t_set *set)
{
	int	i;

	i = 0;
	set->dq = -1;
	set->sq = -1;
	while (set->input[i])
	{
		if (set->input[i] == '\"' && set->sq == -1)
			set->dq *= -1;
		else if (set->input[i] == '\'' && set->dq == -1)
			set->sq *= -1;
		i++;
	}
	if (set->dq == -1 && set->sq == -1)
		return (1);
	return (0);
}

int	find_size_parse(t_set *set)
{
	int		i;
	int		counter;

	i = 0;
	counter = 0;
	while (set->input[i])
	{
		while (set->input[i] == ' ')
			i++;
		if (set->input[i] == '\'')
		{
			i++;
			if (set->input[i] != '\'')
				counter++;
			while (set->input[i] != '\'')
				i++;
			while (set->input[i] != '\0' && set->input[i] != ' ')
				i++;
		}
		else if (set->input[i] == '\"')
		{
			i++;
			if (set->input[i] != '"')
				counter++;
			while (set->input[i] != '\"')
				i++;
			while (set->input[i] != '\0' && set->input[i] != ' ')
				i++;
		}
		else
		{
			counter++;
			while (set->input[i] != ' ' && set->input[i] != '\0')
			{
				if (set->input[i] == '\'' || set->input[i] == '\"')
				{
					i++;
					while (set->input[i] != '\'' && set->input[i] != '\"')
						i++;
				}
				i++;
			}
		}
		while (set->input[i] == ' ')
			i++;
	}
	return (counter);
}

char	**parse(t_set *set)
{
	int		i;
	int		counter;
	char	**split;
	int		g;

	i = 0;
	counter = 0;
	if (check_quotes(set) == 0)
	{
		printf("Quotes not close ! What is happening ?\n");
		return (NULL);
	}
	counter = find_size_parse(set);
	// printf("Le nombre de mot est : %d\n", counter);
	g = 0;
	if (counter == 0)
		return (NULL);
	split = ft_calloc(sizeof(char *), (counter + 1));
	set->i = 0;
	i = set->i;
	while (set->input[i] == ' ')
	{
		set->i += 1;
		i++;
	}
	while (set->input[i] && g < counter)
	{
		// printf("Indice = %d\n", set->i);
		i = set->i;
		while (set->input[i] == ' ')
		{
			set->i += 1;
			i++;
		}
		if (set->input[i] == '\'' || set->input[i] == '\"')
		{
			split[g] = copy_quotes(set);
		}
		else
		{
			split[g] = copy_normal(set);
			// printf("Apres ouen est i : %d\n", set->i);
		}
		// printf("Le tab g%d: %s\n", g, split[g]);
		// set->i += 1;
		i++;
		g++;
	}
	// printf("%d\n", g);
	split[g] = NULL;
	// g = 0;
	// while (split[g])
	// {
	// 	printf("Le tab : |%s|\n", split[g]);
	// 	g++;
	// }
	return (split);
}

size_t	ft_occurence(char *s)
{
	int i;
	size_t compteur;

	i = 0;
	compteur = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		if ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ' || s[i] == '\t')
			compteur++;
		i++;
	}
	return (compteur);
}