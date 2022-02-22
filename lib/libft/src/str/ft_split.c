/*
** Allocates (with malloc) array of strings obtained by splitting 's'
** using the character 'c' as a delimiter.
** The array must be ended by a NULL pointer.
** Returns array of new strings, or NULL if the allocation fails.
*/

#include "libft.h"

static int	count_num(char const *s, char c)
{
	int	ret;

	ret = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
			ret++;
		while (*s && *s != c)
			s++;
	}
	return (ret);
}

static int	alloc_copy_str(char const *s, char c, char **ret, int num)
{
	int		cnt;
	size_t	len;

	cnt = 0;
	while (*s && cnt < num)
	{
		len = 0;
		while (*s && *s == c)
			s++;
		while (*(s + len) && *(s + len) != c)
			len++;
		ret[cnt] = malloc(sizeof(char) * (len + 1));
		if (!ret[cnt])
		{
			while (cnt-- > -1)
				free(ret[cnt]);
			free(ret);
			return (0);
		}
		ft_strlcpy(ret[cnt], s, len + 1);
		cnt++;
		s += len;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**ret;
	int		num;

	if (!s)
		return (NULL);
	num = count_num(s, c);
	ret = malloc(sizeof(char *) * (num + 1));
	if (!ret)
		return (NULL);
	if (!alloc_copy_str(s, c, ret, num))
		return (NULL);
	ret[num] = NULL;
	return (ret);
}
