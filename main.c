/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:27:31 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/20 23:27:33 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	yes_or_no_builtins(t_set *set)
{
	int	i;

	i = 0;
	while (set->cmd[i])
	{
		if (ft_strcmp("echo", set->cmd[i]) == 0)
			return (1);
		else if (ft_strcmp("cd", set->cmd[i]) == 0)
			return (1);
		else if (ft_strcmp("pwd", set->cmd[i]) == 0)
			return (1);
		else if (ft_strcmp("env", set->cmd[i]) == 0)
			return (1);
		else if (ft_strcmp("exit", set->cmd[i]) == 0)
			return (1);
		else if (ft_strcmp("unset", set->cmd[i]) == 0)
			return (1);
		else if (ft_strcmp("export", set->cmd[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

char **find_write(t_set *set)
{
	int	i;
	int	j;
	int	counter;
	char **to_copy;

	counter = 0;
	i = 0;
	j = 0;
	if (set->cmd[i])
	{
		if (ft_strcmp(set->cmd[i], "echo") != 0)
		{
			while (set->cmd[i] && ft_strcmp(set->cmd[i], "echo") != 0)
			{
				i++;
			}	
		}
		i++;
		if (set->dq != 1)
		{
			while (set->cmd[i] && ft_strcmp(set->cmd[i], ">") != 0)
			{
				counter++;
				i++;
			}
			i++;
		}
		else
		{
			while (set->cmd[i])
			{
				counter++;
				i++;
			}
			i++;
		}
	}
	to_copy = malloc(sizeof(char *) * (counter + 1));
	i = 0;
	if (set->cmd[i])
	{
		if (ft_strcmp(set->cmd[i], "echo") != 0)
		{
			while (set->cmd[i] && ft_strcmp(set->cmd[i], "echo") != 0)
			{
				i++;
			}	
		}
		i++;
		if (set->dq != 1)
		{
			while (set->cmd[i] && (ft_strcmp(set->cmd[i], ">") != 0 && ft_strcmp(set->cmd[i], ">>") != 0))
			{
				to_copy[j] = ft_strdup(set->cmd[i]);
				// printf("%s\n", to_copy[j]);
				j++;
				i++;
			}
			i++;
		}
		else
		{
			while (set->cmd[i])
			{
				to_copy[j] = ft_strdup(set->cmd[i]);
				// printf("%s\n", to_copy[j]);
				j++;
				i++;
			}
			i++;
		}
	}
	to_copy[j] = NULL;
	return (to_copy);
}

void	do_builtins(t_set *set)
{
	char **to_write;

	int	i;

	i = 0;
	while (set->cmd[i])
	{
		if (ft_strcmp("echo", set->cmd[i]) == 0)
		{
			to_write = find_write(set);
			set->return_value = echo_command(to_write);
			free_tab(to_write);
		}
		else if (ft_strcmp("cd", set->cmd[i]) == 0)
			set->return_value = cd_command(set, set->cmd);
		else if (ft_strcmp("pwd", set->cmd[i]) == 0)
			set->return_value = pwd_command(set->cmd);
		else if (ft_strcmp("env", set->cmd[i]) == 0)
			set->return_value = env_command(set->env);
		else if (ft_strcmp("exit", set->cmd[i]) == 0)
			set->return_value = exit_command(set, set->cmd[1], set->size_tab);
		else if (ft_strcmp("unset", set->cmd[i]) == 0)
			set->return_value = unset_command(set, set->env);
		else if (ft_strcmp("export", set->cmd[i]) == 0)
			set->return_value = export_command(set);
		i++;
	}
}

void	executable(t_set *set)
{
	int	j;

	j = 0;
	if (check_pipe(set))
		parse_for_pipe(set);
	else
		do_simple_command(set);
}


void	command(char **c, t_set *set)
{	
	int rd;
	char *file_in;
	char *file_out;
	int fd;
	char **cmd;

	set->append = 0;
	set->append = check_append(c);
	rd = check_redirections(c);
	file_in = find_file_in(c);
	file_out = find_file_out(c);
	if (rd)
	{
		if (rd == 1)
		{
			if (file_in)
			{
				fd = open(file_in, O_RDONLY, 0777);
				if (fd == -1)
				{
					error_mess();
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		else if (rd == 3)
			here_doc(set, c[1]);
		if (file_out)
		{
			if (set->append == 1)
			{
				fd = open(file_out, O_WRONLY | O_CREAT | O_APPEND, 0777);
				if (fd == -1)
					error_mess();
			}
			else
			{
				fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0777);
				if (fd == -1)
				{
					error_mess();
				}
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		cmd = copy_tabcmd(c);
		execute_command(set, cmd, set->env);
	}
	else
	{
		execute_command(set, c, set->env);
	}
}

int	check_grammary(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (0);
		i++;
	}
	i = 0;
	if (str[i] == ' ' || str[i] == '|')
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == '|')
		{
			printf("bash: syntax error near unexpected token '|'\n");
			return (1);
		}
	}
	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if (!str[i + 1])
			{
				printf("bash: syntax error near unexpected token 'newline'\n");
				return (1);
			}
			else if (str[i + 1] == '>' || str[i + 1] == '<' || str[i + 1] == '\0')
			{
				i++;
				if (str[i + 1] == '\0')
				{
					printf("bash: syntax error near unexpected token 'newline'\n");
					return (1);
				}
				else
				{
					if (!str[i + 1])
					{
						printf("bash: syntax error near unexpected token '%c'\n", str[i + 1]);
						return (1);
					}
					while (str[i + 1] == ' ')
						i++;
					if (str[i + 1] == '>' || str[i + 1] == '<')
					{
						printf("bash: syntax error near unexpected token '%c'\n", str[i + 1]);
						return (1);
					}
				}
			}
			else if (str[i + 1] == ' ')
			{
				while (str[i + 1] == ' ')
					i++;
				if (str[i + 1] == '>' || str[i + 1] == '<')
				{
					printf("bash: syntax error near unexpected token '%c'\n", str[i + 1]);
					return (1);
				}
			}
		}
		else if(str[i] == '|')
		{
			if (str[i + 1])
			{
				i++;
				while (str[i] == ' ')
					i++;
				if (!str[i])
				{
					printf("bash: syntax error near unexpected token '|'\n");
					return (1);
				}
			}
			else
			{
				printf("bash: syntax error near unexpected token '|'\n");
				return (1);
			}
		}
		if (str[i])
			i++;
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_set	set;
	int		i;

	(void)av;
	(void)env;
	if (ac < 1)
		return (1);
	i = 0;
	set.env = copy_of_tab(env);
	set.return_value = 0;
	set.expand = 0;
	while (1)
	{
		set.input = readline("\1\033[38;5;226m\2M\1\033[38;5;220m\2i\1\033[38;5;214m\2"
								"n\1\033[38;5;208m\2i\1\033[38;5;202m\2S\1\033[38;5;196m\2"
								"c\1\033[38;5;202m\2h\1\033[38;5;208m\2w\1\033[38;5;214m\2"
								"e\1\033[38;5;220m\2p\1\033[38;5;226m\2p\1\033[38;5;220m\2"
								"e\1\033[38;5;214m\2s\1\033[38;5;208m\2 \1\033[38;5;208m\2"
								">\1\033[0m ");
		if (set.input == NULL)
		{
			free_tab(set.env);
			return (0);
		}
		set.cmd = NULL;
		if (ft_occurence(set.input) != ft_strlen(set.input))
		{
			add_history(set.input);
			set.i = 0;
			if (check_grammary(set.input) == 0)
			{
				set.cmd = parse(&set);
			}
		}
		if (set.cmd)
		{
			if (set.expand == 1)
			{
				if (expand(&set) == 0)
					break;
			}
			set.size_tab = tab_calculate(set.cmd);
			executable(&set);
			free_tab(set.cmd);
		}
		free(set.input);
	}
	free_tab(set.env);
	clear_history();
	return (0);
}
