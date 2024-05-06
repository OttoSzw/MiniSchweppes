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