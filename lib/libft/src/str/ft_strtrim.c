/*
** Allocates (with malloc) copy of 's1' with the characters
** specified in 'set' removed from beginning and end of string 's1'.
** Returns trimmed string, or NULL if the allocation fails.
*/

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;

	if (!s1)
		return (NULL);
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	len = ft_strlen(s1);
	while (len && ft_strchr(set, *(s1 + len - 1)))
		len--;
	return (ft_substr(s1, 0, len));
}
