/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oszwalbe <oszwalbe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:27:42 by oszwalbe          #+#    #+#             */
/*   Updated: 2024/05/29 14:44:28 by oszwalbe         ###   ########.fr       */
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
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>

# define COLOR "\1\033[38;5;226m\2M\1\033[38;5;220m\2i\1\033[38;5;214m\2\
n\1\033[38;5;208m\2i\1\033[38;5;202m\2S\1\033[38;5;196m\2\
c\1\033[38;5;202m\2h\1\033[38;5;208m\2w\1\033[38;5;214m\2\
e\1\033[38;5;220m\2p\1\033[38;5;226m\2p\1\033[38;5;220m\2\
e\1\033[38;5;214m\2s\1\033[38;5;208m\2 \1\033[38;5;208m\2\
>\1\033[0m "

typedef struct s_set
{
	char	*input;
	int		*rdd;
	char	**cmd;
	char	**env;
	char	**files;
	char	*file;
	char	***c;
	int		stop;
	int		nb_arg;
	int		nb_case;
	int		round;
	int		size_tab;
	int		need_to_free;
	int		pipe_fd[2];
	int		i;
	int		j;
	pid_t	id;
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

// Utils for Builtins

int			exportlen(char *tab2);
void		swap_strings(char **a, char **b);
int			export_alone(t_set *set, char **c);
void		sort_list_ascii(char **export_env);

//	Utils functions for tab

void		free_tab(char **s);
int			tab_calculate(char **s);
char		**copy_of_tab(char **tab);
int			tab_calculate_no_pipe(char **s);
void		print_tab(char **cmd);
int			count_cmdpipe(char **av);

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
int			do_builtins(t_set *set, char **c);
int			yes_or_no_builtins(t_set *set, char **c);
void		command(char ***s, char **c, t_set *set);
int			redir_or_not(char **av);
void		if_redir(t_set *set, int nb_files, int rd);
void		set_index(t_set *set, int i);
void		set_index2(t_set *set, int i);
void		child_pipe(int *pipe_fd, t_set *set, char ***c, int i);
void		child_pipe2(int *fd_previous, int *pipe_fd, t_set *set, char ***c);
void		child_pipe3(int *fd_previous, t_set *set, char ***c, int i);
int			yes_or_no_builtins(t_set *set, char **c);
char		**find_write(t_set *set, char **cmd);
int			do_builtins(t_set *set, char **c);
void		do_redir(t_set *set, int nb_files, int i);

//	Utils functions for parsing

char		**parse(t_set *set);
size_t		ft_occurence(char *s);
int			find_size_parse(t_set *set);
char		*copy_normal(t_set *set);
void		oueoue(t_set *set, int *i);
void		def(int *counter, int *i);
int			find_size_quotes(t_set *set, int i);
char		*find_arg_quoted(t_set *set, int i, int counter, int block);
void		check_sq_dq(t_set *set);
void		executable(t_set *set);
void		find_write_exec(int *i, char **cmd);
void		pass_the_quote(char *str, int *i);
int			pass_the_pipe(t_set *set, char *str, int *i);
void		init_read(t_set *set);
int			check_gram_in(char *str, int *i);
int			check_gram_out(char *str, int *i);

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
int			expand_norm(t_set *set, int i, int j);
char		*copy_char(t_set *set, int size2, int size, char *tempo);

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
int			ft_strcmp(char *s1, char *s2);
void		PipeBendoNaBendo(t_set *set, char **env);
void		here_doc(t_set *set, char *limiter, char *av2, int file);
int			get_next_line2(char **line);

// Utils for free

void		free_struct(t_set *set);
void		free_struct2(t_set *set);
void		free_tab(char **s);
void		free_paths(char **split);
void		escpapee(t_set *set, char **cmd);

// Signals

void		init_struct(t_set *set, char **env);
void		init_strcut2(t_set *set);
void		handle_sigint(int sig);
void		signals(void);

#endif
