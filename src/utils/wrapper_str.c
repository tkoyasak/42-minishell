#include "minishell.h"

char	*ft_xstrdup(const char *s1)
{
	char	*ret;

	ret = ft_strdup(s1);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xstrndup(const char *s1, size_t len)
{
	char	*ret;

	ret = ft_strndup(s1, len);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xsubstr(char const *s, unsigned int start, size_t len)
{
	char	*ret;

	ret = ft_substr(s, start, len);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xstrjoin(char *s1, char *s2)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (!ret)
		error_handler("malloc");
	return (ret);
}
