# include "../minishell.h"

int	unset_command(t_set *set, char **env)
{
	int	i;

	i = 0;
	if (!set->cmd[1])
		return (0);
	while (env[i])
	{
		if (ft_strncmp(set->cmd[1], env[i], ft_strlen(set->cmd[1])) == 0)
		{
			// free(env[i]);
			while (env[i + 1])
			{
				env[i] = env[i + 1];
				i++;
			}
			env[i] = NULL;
			return (0);
		}
		i++;
	}
	return (0);
}