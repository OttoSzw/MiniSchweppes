#include "../minishell.h"

int	new_cmd(t_set *set, int nb, t_expand *expand)
{
	char	*newcmd;
	char	*tmp;

	if (expand->before)
		tmp = ft_strjoin(expand->before, expand->expand);
	else
		tmp = ft_strdup(expand->expand);
	if (!tmp)
		return (0);
	if (expand->after)
		newcmd = ft_strjoin(tmp, expand->after);
	else
		newcmd = ft_strdup(tmp);
	free(tmp);
	free_expand(expand);
	if (!newcmd)
		return (0);
	free(set->cmd[nb]);
	set->cmd[nb] = ft_strdup(newcmd);
	free(newcmd);
	if (!set->cmd[nb])
		return (0);
	return (1);
}

int	new_arg(t_set *set, int nb, t_expand *expand)
{
	char	*tmp;
	char	*variable;
	int		i;

	i = 0;
	tmp = ft_strtrim(expand->expand, "/$");
	// printf("%s\n", tmp);
	if (!tmp)
		return (0);
	variable = ft_strjoin(tmp, "=");
	free(tmp);
	if (!variable)
		return (0);
	while (set->env[i])
	{
		if (ft_strncmp(variable, set->env[i], env_len(set->env[i])) == 0)
		{
			free(expand->expand);
			expand->expand = ft_substr(set->env[i], ft_strlen(variable),
					(ft_strlen(set->env[i]) - ft_strlen(variable)));
			free(variable);
			if (!expand->expand)
				return (0);
			if (new_cmd(set, nb, expand) == 0)
				return (0);
			return (1);
		}
		i++;
	}
	// printf("1\n");
	free(variable);
	return (1);
}

int	expand_arg(t_set *set, int nb)
{
	int			i;
	char		*tmp;
	t_expand	expand;

	init_expand(&expand);
	i = 0;
	while (set->cmd[nb][i] && set->cmd[nb][i] != '$')
		i++;
	expand.before = copy_str(set->cmd[nb], i);
	if (!expand.before)
		return (0);
	tmp = ft_substr(set->cmd[nb], ft_strlen(expand.before),
			ft_strlen(set->cmd[nb]));
	if (!tmp)
		return (0);
	i = 0;
	while (tmp[i] == '$')
		i++;
	while ((tmp[i] >= 'A' && tmp[i] <= 'Z') || (tmp[i] >= 'a' && tmp[i] <= 'z')
		|| tmp[i] == '_')
		i++;
	expand.expand = copy_str(tmp, i);
	if (!expand.expand)
		return (free(tmp), 0);
	if (tmp[i])
	{
		expand.after = ft_substr(tmp, i, (ft_strlen(tmp)
					- ft_strlen(expand.expand)));
		if (!expand.after)
		{
			return (0);
		}
	}
	free(tmp);
	if (new_arg(set, nb, &expand) == 0)
		return (free_expand(&expand), 0);
	return (1);
}

int	expand(t_set *set)
{
	int i;
	int j;

	i = 0;
	while (set->cmd[i])
	{
		j = 0;
		while (set->cmd[i][j])
		{
			if (set->cmd[i][j] == '\'' && set->cmd[i][j + 1] == '$')
				return (1);
			else if (set->cmd[i][j] == '$')
			{
				if (expand_arg(set, i) == 0)
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}