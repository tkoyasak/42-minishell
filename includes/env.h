#ifndef ENV_H
# define ENV_H

# include "libft.h"

typedef struct s_item
{
	char			*key;
	char			*val;
}	t_item;

t_list		*init_env(void);


#endif