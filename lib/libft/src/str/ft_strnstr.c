/*
** Locates first occurrence of 'needle' in 'haystack',
** where not more than len characters are searched.
** Characters that appear after `\0' character are not searched.
** Returns values:
**  1) haystack, if needle is empty string.
**  2) NUL, if needle occurs nowhere in haystack.
**  3) otherwise, a pointer to first character of first occurrence of 'needle'.
*/

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needlelen;

	needlelen = ft_strlen(needle);
	if (!needlelen)
		return ((char *)haystack);
	while (*haystack && len-- >= needlelen)
	{
		if (!ft_strncmp(haystack, needle, needlelen))
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
