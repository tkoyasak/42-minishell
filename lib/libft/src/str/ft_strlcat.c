/*
** Appends string 'src' to end of 'dst'
** at most 'dstsize - strlen(dst) - 1' characters.
** Then NUL-terminate,
** unless 'dstsize' is 0 or original 'dst' string was longer than 'dstsize'.
** Returns total length of string they tried to create.
** :initial length of 'dst' plus length of 'src'
** If return value is >= 'dstsize', output string has been truncated.
*/

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dstlen;
	size_t	srclen;
	size_t	i;

	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if (dstlen >= dstsize)
		return (dstsize + srclen);
	i = 0;
	while (src[i] && i + dstlen < dstsize - 1)
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	dst[dstlen + i] = '\0';
	return (dstlen + srclen);
}
