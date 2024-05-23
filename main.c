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

int		g_signal;

int	yes_or_no_builtins(t_set *set, char **c)
{
	int	i;

	i = 0;
	(void)set;
	while (c[i])
	{
		if (ft_strcmp("echo", c[i]) == 0)
			return (1);
		else if (ft_strcmp("cd", c[i]) == 0)
			return (1);
		else if (ft_strcmp("pwd", c[i]) == 0)
			return (1);
		else if (ft_strcmp("env", c[i]) == 0)
			return (1);
		else if (ft_strcmp("exit", c[i]) == 0)
			return (1);
		else if (ft_strcmp("unset", c[i]) == 0)
			return (1);
		else if (ft_strcmp("export", c[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

char	**find_write(t_set *set, char **cmd)
{
	int		i;
	int		j;
	int		counter;
	char	**to_copy;

	counter = 0;
	i = 0;
	j = 0;
	if (cmd[i])
	{
		if (ft_strcmp(cmd[i], "echo") != 0)
		{
			while (cmd[i] && ft_strcmp(cmd[i], "echo") != 0)
			{
				i++;
			}
		}
		i++;
		if (set->dq != 1)
		{
			while (cmd[i] && ft_strcmp(cmd[i], ">") != 0)
			{
				counter++;
				i++;
			}
			i++;
		}
		else
		{
			while (cmd[i])
			{
				counter++;
				i++;
			}
			i++;
		}
	}
	to_copy = malloc(sizeof(char *) * (counter + 1));
	i = 0;
	if (cmd[i])
	{
		if (ft_strcmp(cmd[i], "echo") != 0)
		{
			while (cmd[i] && ft_strcmp(cmd[i], "echo") != 0)
			{
				i++;
			}
		}
		i++;
		if (set->dq != 1)
		{
			while (cmd[i] && (ft_strcmp(cmd[i], ">") != 0 && ft_strcmp(cmd[i],
						">>") != 0))
			{
				to_copy[j] = ft_strdup(cmd[i]);
				// printf("%s\n", to_copy[j]);
				j++;
				i++;
			}
			i++;
		}
		else
		{
			while (cmd[i])
			{
				to_copy[j] = ft_strdup(cmd[i]);
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

void	do_builtins(t_set *set, char **c)
{
	char	**to_write;
	int		i;

	i = 0;
	while (c[i])
	{
		if (ft_strcmp("echo", c[i]) == 0)
		{
			to_write = find_write(set, c);
			set->return_value = echo_command(to_write);
			free_tab(to_write);
		}
		else if (ft_strcmp("cd", c[i]) == 0)
			set->return_value = cd_command(set, c);
		else if (ft_strcmp("pwd", c[i]) == 0)
			set->return_value = pwd_command(c);
		else if (ft_strcmp("env", c[i]) == 0)
			set->return_value = env_command(set->env);
		else if (ft_strcmp("exit", c[i]) == 0)
			set->return_value = exit_command(set, c[1], tab_calculate(c));
		else if (ft_strcmp("unset", c[i]) == 0)
			set->return_value = unset_command(set, set->env);
		else if (ft_strcmp("export", c[i]) == 0)
			set->return_value = export_command(set, c, tab_calculate(c));
		i++;
	}
}

void	executable(t_set *set)
{
	int	j;

	j = 0;
	if (check_pipe(set))
	{
		set->flag_pipe = 1;
		parse_for_pipe(set);
	}
	else
		do_simple_command(set);
}

void	command(char ***s, char **c, t_set *set)
{
	int		rd;
	char	*file;
	int		fd;
	char	**cmd;
	int		i;
	int		j;
	int		nb_files;

	i = 0;
	set->append = 0;
	set->index = 0;
	set->index2 = 0;
	rd = redir_or_not(c);
	nb_files = count_nb_files(c);
	if (rd && (set->dq != 1 && set->sq != 1))
	{
		set->files = malloc(sizeof(char *) * (nb_files + 1));
		set->rdd = malloc(sizeof(int) * (nb_files + 1));
		if (!set->files)
			return ;
		while (i < nb_files)
		{
			file = find_file_out2(set, c);
			rd = check_redirections(set, c);
			if (file)
			{
				set->files[i] = ft_strdup(file);
				set->rdd[i] = rd;
			}
			i++;
		}
		set->files[i] = NULL;
		set->rdd[i] = -1;
		i = 0;
		while (i < nb_files)
		{
			if (set->rdd[i] == 1 || set->rdd[i] == 3)
			{
				fd = open(set->files[i], O_RDONLY);
				if (fd == -1)
				{
					j = 0;
					free_struct2(set);
					while (s[j])
					{
						free_tab(s[j]);
						j++;
					}
					free(s);
					close(set->saved_in);
					close(set->saved_out);
					error_mess();
				}
				dup2(fd, STDIN_FILENO);
				if (set->rdd[i] == 3)
					unlink(set->files[i]);
				close(fd);
			}
			else if (set->rdd[i] == 4)
			{
				fd = open(set->files[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd == -1)
					error_mess();
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else if (set->rdd[i] == 2)
			{
				fd = open(set->files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
					error_mess();
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			i++;
		}
		if (yes_or_no_builtins(set, set->cmd) == 1)
		{
			do_builtins(set, c);
			free_struct(set);
			exit(set->return_value);
		}
		else
		{
			cmd = copy_tabcmd(set, c);
			if (!cmd)
			{
				close(set->saved_in);
				free_struct(set);
				close(set->saved_out);
				exit(1);
			}
			set->need_to_free = 1;
			close(set->saved_in);
			close(set->saved_out);
			execute_command(set, cmd, set->env);
		}
	}
	else
	{
		if (yes_or_no_builtins(set, c) == 1)
		{
			do_builtins(set, c);
			free_struct(set);
			exit(set->return_value);
		}
		else
		{
			close(set->saved_in);
			close(set->saved_out);
			execute_command(set, c, set->env);
		}
	}
}

int	check_grammary(t_set *set, char *str)
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
			printf("bash: syntax error near unexpected token `|'\n");
			return (set->return_value = 2, 2);
		}
	}
	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if (!str[i + 1])
			{
				printf("bash: syntax error near unexpected token `newline'\n");
				return (set->return_value = 2, 2);
			}
			else if (str[i + 1] == '>' || str[i + 1] == '<' || str[i
				+ 1] == '\0')
			{
				i++;
				if (str[i + 1] == '\0')
				{
					printf("bash: syntax error near unexpected token `newline'\n");
					return (set->return_value = 2, 2);
				}
				else
				{
					if (!str[i + 1])
					{
						printf("bash: syntax error near unexpected token `%c'\n",
							str[i + 1]);
						return (set->return_value = 2, 2);
					}
					while (str[i + 1] == ' ')
						i++;
					if (str[i + 1] == '>' || str[i + 1] == '<')
					{
						printf("bash: syntax error near unexpected token `%c'\n",
							str[i + 1]);
						return (set->return_value = 2, 2);
					}
				}
			}
			else if (str[i + 1] == ' ')
			{
				while (str[i + 1] == ' ')
					i++;
				if (str[i + 1] == '>' || str[i + 1] == '<')
				{
					printf("bash: syntax error near unexpected token `%c'\n",
						str[i + 1]);
					return (set->return_value = 2, 2);
				}
			}
		}
		else if (str[i] == '|')
		{
			if (str[i + 1])
			{
				i++;
				while (str[i] == ' ')
					i++;
				if (!str[i])
				{
					printf("bash: syntax error near unexpected token `|'\n");
					return (set->return_value = 2, 2);
				}
				else if (str[i] == '|')
				{
					printf("bash: syntax error near unexpected token `|'\n");
					return (set->return_value = 2, 2);
				}
				else if (str[i] == '>' || str[i] == '<')
				{
					printf("bash: syntax error near unexpected token `|'\n");
					return (set->return_value = 2, 2);
				}
			}
			else
			{
				printf("bash: syntax error near unexpected token `|'\n");
				return (set->return_value = 2, 2);
			}
		}
		if (str[i])
			i++;
	}
	return (0);
}

char	*do_here_doc(t_set *set, int nb, char *limiter, char *and)
{
	char	*number;
	char	*file;
	int		fd;

	number = ft_itoa(nb);
	file = ft_strdup(".infile_");
	set->file = ft_strjoin(file, number);
	free(number);
	free(file);
	fd = open(set->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("Chef c est mauvais la\n");
		return (NULL);
	}
	here_doc(set, limiter, and, fd);
	close(fd);
	reset_fd(set);
	return (set->file);
}

void	here_doggy(t_set *set)
{
	int		i;
	int		nb;
	char	*file;

	i = 0;
	nb = 0;
	while (set->cmd[i])
	{
		if (ft_strcmp("<<", set->cmd[i]) == 0)
		{
			i++;
			file = do_here_doc(set, nb, set->cmd[i], set->cmd[i + 1]);
			free(set->cmd[i]);
			set->cmd[i] = ft_strdup(file);
			free(set->file);
			nb++;
		}
		i++;
	}
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
	g_signal = 0;
	init_struct(&set, env);
	while (1)
	{
		// printf("%d\n", g_signal);
		set.expand = 0;
		set.flag_pipe = 0;
		g_signal = 0;
		// printf("signal = %d\n", g_signal);
		// printf("return = %d\n", set.return_value);
		set.input = readline("\1\033[38;5;226m\2M\1\033[38;5;220m\2i\1\033[38;5;214m\2"
							"n\1\033[38;5;208m\2i\1\033[38;5;202m\2S\1\033[38;5;196m\2"
							"c\1\033[38;5;202m\2h\1\033[38;5;208m\2w\1\033[38;5;214m\2"
							"e\1\033[38;5;220m\2p\1\033[38;5;226m\2p\1\033[38;5;220m\2"
							"e\1\033[38;5;214m\2s\1\033[38;5;208m\2 \1\033[38;5;208m\2"
							">\1\033[0m ");
		if (g_signal == 130)
		{
			set.return_value = g_signal;
			g_signal = 0;
		}
		if (set.input == NULL)
		{
			free_tab(set.env);
			printf("exit");
			return (0);
		}
		set.cmd = NULL;
		if (ft_occurence(set.input) != ft_strlen(set.input))
		{
			add_history(set.input);
			set.i = 0;
			if (check_grammary(&set, set.input) == 0)
			{
				set.cmd = parse(&set);
				if (set.cmd != NULL)
					here_doggy(&set);
			}
		}
		if (set.cmd)
		{
			if (set.expand == 1)
			{
				if (expand(&set) == 0)
					break ;
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
