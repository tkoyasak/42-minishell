/*
** Allocates (with malloc) string representing integer received as argument.
** Negative numbers must be handled.
** Returns string representing the integer, or NULL if the allocation fails.
*/

#include "libft.h"

static int	count_size(int n)
{
	size_t	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*ret;
	size_t	i;

	i = count_size(n);
	ret = malloc(sizeof(char) * (i + 1));
	if (!ret)
		return (NULL);
	ret[i] = '\0';
	if (n == 0)
		ret[0] = '0';
	else if (n < 0)
		ret[0] = '-';
	else if (n > 0)
		n *= -1;
	while (n != 0)
	{
		i--;
		ret[i] = (n % 10) * (-1) + '0';
		n /= 10;
	}
	return ((char *)ret);
}
