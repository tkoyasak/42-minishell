#include "minishell.h"

bool	match_given_pattern(char *str, char *pattern)
{
	bool	**dp;
	size_t	i;
	size_t	j;
	size_t	str_len;
	size_t	ptn_len;

	str_len = ft_strlen(str);
	ptn_len = ft_strlen(pattern);
	dp = ft_calloc(str_len + 1, sizeof(bool *));
	i = -1;
	while (++i <= str_len)
		dp[i] = ft_calloc(ptn_len + 1, sizeof(bool));
	dp[0][0] = true;
	j = 0;
	while (++j <= ptn_len)
	{
		if (pattern[j - 1] == '*' && pattern[j] != '.')
			dp[0][j] = dp[0][j - 1];
	}
	i = 0;
	while (++i <= str_len)
	{
		j = 0;
		while (++j <= ptn_len)
		{
			if (pattern[j - 1] == '*' && !(str[i - 1] == '.' && i * j == 1))
				dp[i][j] = dp[i - 1][j] | dp[i][j - 1];
			else if (pattern[j - 1] == str[i - 1])
				dp[i][j] = dp[i - 1][j - 1];
			else
				dp[i][j] = false;
		}
	}
	return (dp[str_len][ptn_len]);
}
