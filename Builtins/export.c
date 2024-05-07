#include "../minishell.h"

char	**ft_realloc(char **env, int size)
{
	char **env2;
	int	i;

	i = 0;
	env2 = (char **)ft_calloc(sizeof(char *), (size + 1));
	while (env[i])
	{
		env2[i] = ft_strdup(env[i]);
		i++;
	}
	env2[size] = NULL;
	return (env2);
}

int	check_arg(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
	{
		if (str[i] > '0' && str[i] < '9')
			return (0);
		i++;
	}
	return (1);
}

void	insert_sorted(char **sorted_list, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (sorted_list[i] && ft_strcmp(sorted_list[i], str) < 0)
		i++;
	j = i;
	while (sorted_list[j])
		j++;
	while (j > i)
	{
		sorted_list[j] = sorted_list[j - 1];
		j--;
	}
	sorted_list[i] = str;
}

char **sort_list(char **export_env)
{
	int	size;
	char **sorted_env;
	int	i;

	i = 0;
	size = 0;
	while (export_env[size])
		size++;
	sorted_env = malloc((size + 1) * sizeof(char *));
	if (!sorted_env)
	{
		printf("Boum!!\n");
		return (NULL);
	}
	sorted_env[0] = NULL;
	while (i < size)
	{
		insert_sorted(sorted_env, export_env[i]);
		i++;
	}
	return (sorted_env);
}

int	success_find(char **env, char *tab2)
{
	char *str;
	int i;
	int size;

	i = 0;
	size = 0;
	while (tab2[size] != '=')
		size++;
	str = malloc(sizeof(char) * (size + 1));
	i = 0;
	while (tab2[i] != '=')
	{
		str[i] = tab2[i];
		i++;
	}
	str[i] = '\0';
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], str, size) == 0)
		{
			free(env[i]);
			env[i] = ft_strdup(tab2);
			return (free(str), 1);
		}
		i++;
	}
	return (free(str), 0);
}

int	export_command(t_set *set, char **env)
{
	char **export_env;
	int	i;
	int j;
	int	size;

	i = 0;
	j = 0;
	size = tab_calculate(env);
	if (!set->cmd[1])
	{
		export_env = copy_of_tab(env);
		export_env = sort_list(export_env);
		print_tab(export_env);
		free_tab(export_env);
		return (0);
	}
	i++;
	if (set->size_tab >= 2)
	{
		if (check_arg(set->cmd[1]))
		{
			if (!(success_find(set->env, set->cmd[1])))
			{
				set->env = ft_realloc(env, size);
				set->env[size] = ft_strdup(set->cmd[1]);
				size++;
				set->env[size] = NULL;
			}
		}
		else
		{
			printf("'%s' : not a valid identifier\n", set->cmd[i]);
		}
	}
	return (0);
}