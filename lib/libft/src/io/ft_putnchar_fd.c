#include "libft.h"

void	ft_putnchar_fd(char *s, size_t n, int fd)
{
	size_t	idx;

	idx = -1;
	while (++idx < n)
		ft_putchar_fd(s[idx], fd);
}
