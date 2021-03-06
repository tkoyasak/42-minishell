/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:07:20 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/08 14:31:38 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANALYZER_H
# define ANALYZER_H

# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <dirent.h>
# include "libft.h"
# include "struct.h"
# include "shell_var.h"

# define RESERVED_CHAR "<>|&;"
# define PROC_DELIM_CHAR "|&;"
# define REDIRECT_CHAR "<>"
# define QUOTE_CHAR "\"\'"
# define PAREN_CHAR "()"

# define FD_NONE -1

/* lexer */
int		lexer(char *line, t_list **token_list);
bool	set_token_len_and_kind(char *p, int *len, t_token_kind *kind);
void	lexer_error(char *p, int idx);

/* parser */
int		parser(t_node **tree, t_list *token_list);
void	parser_error(t_list **itr, char *str, bool *is_valid);
t_node	*node_new(t_node_kind kind, t_node *lhs, t_node *rhs);
bool	consume_node_kind(t_list **itr, char *op, bool *is_valid);
t_node	*create_proc_node(t_list **itr, bool *is_valid);
t_node	*convert_to_expr_tree(t_node *tree);

/* expander */
void	expander(t_expr *expr, t_sh_var *sh_var);
t_list	*get_expanded_token_list(char *str, t_sh_var *sh_var);
t_list	*split_by_expd_kind(char *str, bool par_in_dquote, bool heredoc);
t_list	*expand_env(t_list *expd_list, t_sh_var *sh_var, bool heredoc);
t_list	*extract_word(char **str, bool in_squote, bool in_dquote, \
														t_expd_kind kind);
t_list	*create_zero_str(bool in_squote, bool in_dquote, t_expd_kind kind);
t_list	*remove_quotes(t_list *src_list);
t_list	*convert_to_token_list(t_list *expd_list);
t_list	*expand_wildcard(t_list *expd_list);
t_list	*matched_files(char *prefix, char **slash_splitted_strs);
bool	match_pattern(char *str, char *pattern);

#endif
