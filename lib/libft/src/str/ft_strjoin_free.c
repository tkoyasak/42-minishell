#include "libft.h"

char	*ft_strjoin_free(char *s1, char *s2, bool is_s2_free)
{
	char	*ret;
	size_t	len;
	char	*tmp1;
	char	*tmp2;

	if (!s1 || !s2)
		return (NULL);
	tmp1 = s1;
	tmp2 = s2;
	len = ft_strlen(s1) + ft_strlen(s2);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	while (*s1)
		*ret++ = *s1++;
	while (*s2)
		*ret++ = *s2++;
	*ret = '\0';
	free(tmp1);
	if (is_s2_free)
		free(tmp2);
	return (ret - len);
}
