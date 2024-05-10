#include "../minishell.h"

int	env_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	i++;
	return (i);
}

char	*copy_str(char *str, int size)
{
	char	*newstr;

	newstr = malloc(sizeof(char) * (size + 1));
	if (!newstr)
		return (NULL);
	ft_strlcpy(newstr, str, size + 1);
	return (newstr);
}

void	init_expand(t_expand *expand)
{
	expand->before = NULL;
	expand->expand = NULL;
	expand->after = NULL;
}

void	free_expand(t_expand *expand)
{
	if (expand->before)
		free(expand->before);
	if (expand->expand)
		free(expand->expand);
	if (expand->after)
		free(expand->after);
}

int	remove_quote(t_set *set, int nb)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	tmp = malloc(sizeof(char) * (ft_strlen(set->cmd[nb]) + 1));
	if (!tmp)
		return (0);
	while (set->cmd[nb][i])
	{
		while (set->cmd[nb][i] && set->cmd[nb][i] != '\'')
			tmp[j++] = set->cmd[nb][i++];
		if (set->cmd[nb][i])
			i++;
	}
	tmp[j] = '\0';
	free(set->cmd[nb]);
	set->cmd[nb] = ft_substr(tmp, 0, ft_strlen(tmp));
	free(tmp);
	if (!set->cmd[nb])
		return (0);
	return (1);
}