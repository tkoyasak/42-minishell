/*
** Allocates (with malloc) substring from string 's'.
** The substring begins at index 'start' and is of maximum size 'len'.
** Returns substring, or NUL if the allocation fails.
*/

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start > slen)
		return (ft_strdup(""));
	if (len + start > slen)
		len = slen - start;
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, s + start, len + 1);
	return ((char *)ret);
}
