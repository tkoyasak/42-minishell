/*
** Converts a lower-case letter to the corresponding upper-case letter.
** Returns the corresponding upper-case letter if there is one;
** otherwise, the argument is returned unchanged.
*/

#include "libft.h"

int	ft_toupper(int c)
{
	if ('a' <= c && c <= 'z')
		return (c - 32);
	else
		return (c);
}
