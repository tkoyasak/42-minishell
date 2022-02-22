/*
** Converts a upper-case letter to the corresponding lower-case letter.
** Returns the corresponding lower-case letter if there is one;
** otherwise, the argument is returned unchanged.
*/

#include "libft.h"

int	ft_tolower(int c)
{
	if ('A' <= c && c <= 'Z')
		return (c + 32);
	else
		return (c);
}
