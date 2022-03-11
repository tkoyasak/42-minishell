#ifndef ENV_H
# define ENV_H

# include "libft.h"

typedef struct s_env
{
	char			*key;
	char			*val;
}	t_env;

t_list		*init_envlist(void);
char		**get_environ(void);


#endif