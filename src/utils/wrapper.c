#include "minishell.h"

void	*ft_xmalloc(size_t size)
{
	void	*buff;

	buff = malloc(size);
	if (!buff)
		error_handler("malloc");
	return (buff);
}

void	*ft_xcalloc(size_t count, size_t size)
{
	void	*buff;

	buff = ft_calloc(count, size);
	if (!buff)
		error_handler("malloc");
	return (buff);
}

char	**ft_xsplit(char *s, char c)
{
	char	**ret;

	ret = ft_split(s, c);
	if (!ret)
		error_handler("malloc");
	return (ret);
}

char	*ft_xitoa(int n)
{
	char	*ret;

	ret = ft_itoa(n);
	if (!ret)
		error_handler("malloc");
	return (ret);
}
