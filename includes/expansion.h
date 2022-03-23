#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>
# include <dirent.h>
# include "libft.h"
# include "struct.h"
# include "parser.h"
# include "shell_var.h"

void			expansion(t_expr *expr, t_shell_var *shell_var);
t_list			*get_expansion_list(char *str, bool par_in_dquote, t_shell_var *shell_var);
t_list			*remove_quotes(t_list *src_list);
t_list			*convert_to_token_list(t_list *expansion_list);
t_list			*get_filename_expansion(t_list *expansion_list);
bool			match_given_pattern(char *str, char *pattern);

#endif
