#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>

# include "parser.h"
# include "libft.h"

typedef enum e_exp_strlist_type
{
	SQUOTE,
	DQUOTE,
	ENV,
	STRING,
	NAKED_SPACE
}	t_exp_strlist_type;

typedef struct s_exp_strlist
{
	int						len;
	char					*str;
	bool					in_squote;
	bool					in_dquote;
	t_exp_strlist_type		type;
	struct s_exp_strlist	*next;
}	t_exp_strlist;

t_node			*expansion(char *argv);
t_exp_strlist	*get_exp_strlist(char *str, bool par_in_dquote);
t_exp_strlist	*remove_quotes(t_exp_strlist *src_list);

#endif
