#include "libft.h"

char	*ft_strndup(const char *s, size_t len)
{
	char	*ret;

	ret = malloc(len + 1);
	if (!ret)
		return (NULL);
	ft_memcpy(ret, s, len);
	*(ret + len) = '\0';
	return (ret);
}
