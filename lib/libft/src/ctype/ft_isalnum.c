/*
** Returns zero if 'c' tests false,
** or returns non-zero if 'c' tests true.
*/

#include "libft.h"

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
