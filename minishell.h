/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:27:42 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/04/20 23:27:44 by oszwalbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>

typedef struct pipex
{
	int		i;
	int		yes;
	int		pipe_fd[2];
	int		saved_in;
	int		saved_out;
	pid_t	id;
}			t_pipex;

typedef struct s_set
{
	char	*input;
	char	**cmd;
	char	**env;
	int		size_tab;
	int		i;
	char	*for_fd;
	char	**for_write;
	int		saved_in;
	int		saved_out;
	int		append;
	int		index;
}			t_set;

// Minishell.h

//	Builtins functions

int			echo_command(char **s);
int			env_command(char **s, char **env);
int			pwd_command(char **s);
int			cd_command(char *path);
int			exit_command(char *s, int size);
int			unset_command(t_set *set, char **env);

//	Utils functions for tab

void		free_tab(char **s);
int			tab_calculate(char **s);
char		**copy_of_tab(char **tab);
int			tab_calculate_no_pipe(char **s);

//	Utils functions for execution

void		execute_command(char **av, char **env);
void		do_simple_command(t_set *set);
void		print_tab(char **cmd);
char		***copy_of_tab_of_tab(t_set *set, char **tab);
void		parse_for_pipe(t_set *set);

//	Utils functions for parsing

char		**parse(t_set *set);
size_t		ft_occurence(char *s);

// Utils for Pipex

void		invalid_arguments(void);
void		invalid_arguments2(void);
int			error_mess(void);
void		error_cmd(void);
void		execute_command2(char *av, char **env);
char		*find_path(char *cmd, char **env);
void		escape(char *path);
void		escape2(char *path, char **cmd);
void		free_paths(char **split);
void		child(char *av, char *av2, int *pipe_fd, char **env,
				t_pipex *pipex);
void		child2(char *av, int *pipe_fd, char **env);
void		child3(char *av, char *av2, int *pipe_fd, char **env,
				t_pipex *pipex);
void		do_child(int ac, char **av, t_pipex *pipex, char **env);
void		exec_child(int ac, char **av, t_pipex *pipex, char **env);
int			ft_strcmp(char *s1, char *s2);
void		PipeBendoNaBendo(t_set *set, char **env);
int			check_redirections(char **av);
void		here_doc(char *limiter);

#endif
