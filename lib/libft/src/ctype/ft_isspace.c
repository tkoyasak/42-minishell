#include "libft.h"

int	ft_isspace(int c)
{
	return ((9 <= c && c <= 12) || c == 32);
}
