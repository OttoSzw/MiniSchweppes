
#include "../minishell.h"

int	expand_after(char *tmp, int i, t_expand *expand)
{
	expand->expand = copy_str(tmp, i);
	if (!expand->expand)
		return (0);
	if (tmp[i])
	{
		expand->after = ft_substr(tmp, i, (ft_strlen(tmp)
					- ft_strlen(expand->expand)));
		if (!expand->after)
			return (0);
	}
	return (1);
}

void	free_cmd(t_set *set, int nb)
{
	free(set->cmd[nb]);
	set->cmd[nb] = NULL;
}

void	replace_expand(t_set *set, int i, t_expand *expand, char *variable)
{
	free(expand->expand);
	expand->expand = ft_substr(set->env[i], ft_strlen(variable),
			(ft_strlen(set->env[i]) - ft_strlen(variable)));
}

char	*prep_variable(t_expand *expand)
{
	char	*variable;
	char	*tmp;

	tmp = ft_strtrim(expand->expand, "/$");
	if (!tmp)
		return (NULL);
	variable = ft_strjoin(tmp, "=");
	free(tmp);
	if (!variable)
		return (NULL);
	return (variable);
}

int	expand_return(t_set *set, int nb)
{
	int	size;
	int	size2;
	char *tempo;
	char *tempo2;
	char *copy;
	int	i;
	int	j;

	i = 0;
	j = 0;
	tempo = ft_itoa(set->return_value);
	tempo2 = ft_strdup(set->cmd[nb]);
	free(set->cmd[nb]);
	size = ft_strlen(tempo);
	size2 = ft_strlen(tempo2);
	if (size)
	{
		if (size == 1)
			copy = malloc(sizeof(char) * (size2));
		else if (size == 2)
			copy = malloc(sizeof(char) * (size2 + 1));
		else if (size == 3)
			copy = malloc(sizeof(char) * (size2 + 2));
		while (tempo2[i] != '$')
		{
			copy[j] = tempo2[i];
			j++;
			i++;
		}
		if (size == 1)
			copy[j] = tempo[0];
		else if (size == 2)
		{
			copy[j] = tempo[0];
			j++;
			copy[j] = tempo[1];
		}
		else if (size == 3)
		{
			copy[j] = tempo[0];
			j++;
			copy[j] = tempo[1];
			j++;
			copy[j] = tempo[2];
		}
		i++;
		j++;
		while (tempo2[i] && tempo2[i + 1])
		{
			copy[j] = tempo2[i + 1];
			j++;
			i++;
		}
		copy[j] = '\0';
		set->cmd[nb] = ft_strdup(copy);
	}
	if (!set->cmd[nb])
		return (0);
	free(copy);
	free(tempo2);
	free(tempo);
	return (1);
}