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
# include <signal.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/signal.h>
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
	int		*rdd;
	char	**cmd;
	char	**env;
	char	**files;
	char	*file;
	char	***c;
	int		round;
	int		size_tab;
	int		need_to_free;
	int		i;
	int		saved_in;
	int		saved_out;
	int		saved_fd;
	int		append;
	int		index;
	int		index2;
	int		dq;
	int		sq;
	int		return_value;
	int		expand;
	int		flag_pipe;
}			t_set;

typedef struct s_expand
{
	char	*before;
	char	*after;
	char	*expand;
}			t_expand;
// Minishell.h

//	Builtins functions

int			echo_command(char **s);
int			env_command(char **env);
int			pwd_command(char **s);
int			cd_command(t_set *set, char **path);
int			exit_command(t_set *set, char *s, int size);
int			unset_command(t_set *set, char **env);
int			export_command(t_set *set, char **c, int size);

//	Utils functions for tab

void		free_tab(char **s);
int			tab_calculate(char **s);
char		**copy_of_tab(char **tab);
int			tab_calculate_no_pipe(char **s);
void		print_tab(char **cmd);

//	Utils functions for execution

void		execute_command(t_set *set, char **av, char **env);
void		do_simple_command(t_set *set);
void		print_tab(char **cmd);
char		***copy_of_tab_of_tab(t_set *set, char **tab);
void		parse_for_pipe(t_set *set);
void		print_tab(char **cmd);
void		reset_fd(t_set *set);
void		init_fd(t_set *set);
int			check_redirections(t_set *set, char **av);
int			check_append(char **cmd);
int			count_cmdpipe(char **av);
char		**copy_tabcmd(t_set *set, char **cmd);
void		do_builtins(t_set *set, char **c);
int			yes_or_no_builtins(t_set *set, char **c);
void		command(char ***s, char **c, t_set *set);
int			redir_or_not(char **av);

//	Utils functions for parsing

char		**parse(t_set *set);
size_t		ft_occurence(char *s);

// Expand functions

int			expand(t_set *set);
int			expand_arg(t_set *set, int nb);
int			new_arg(t_set *set, int nb, t_expand *expand);
int			new_cmd(t_set *set, int nb, t_expand *expand);
char		*copy_str(char *str, int size);
int			env_len(char *str);
void		free_expand(t_expand *expand);
void		init_expand(t_expand *expand);
int			remove_quote(t_set *set, int nb);
int			expand_return(t_set *set, int nb);
char		*prep_variable(t_expand *expand);
void		replace_expand(t_set *set, int i, t_expand *expand, char *variable);
void		free_cmd(t_set *set, int nb);
int			expand_after(char *tmp, int i, t_expand *expand);
int			expand_quote(t_set *set, int nb);

// Utils for Files
int			check_append(char **cmd);
char		*find_file_out(char **cmd);
char		*find_file_in(char **cmd);
char		*find_file_in2(t_set *set, char **cmd);
int			count_nb_files(char **av);
char		*find_file_out2(t_set *set, char **cmd);

// Utils for Pipe

int			check_pipe(t_set *set);
void		exec_multiple_pipe(char ***c, t_set *set, int size);
void		parse_for_pipe(t_set *set);

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
void		here_doc(t_set *set, char *limiter, char *av2, int file);

// Utils for free and init

void		init_struct(t_set *set, char **env);
void		free_struct(t_set *set);
void		free_struct2(t_set *set);

//Signals

void	handle_sigint(int sig);

#endif
