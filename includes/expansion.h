#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>

# include "parser.h"
# include "libft.h"

typedef struct s_exp_strlist
{
	bool			in_dquote;
	int				len;
	char			*str;
	struct s_exp_strlist	*next;
}	t_exp_strlist;

t_node	*expansion(char *argv);

#endif
