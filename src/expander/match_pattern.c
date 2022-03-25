/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_pattern.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:23 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/24 11:17:24 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_index {
	STR,
	PATTERN
};

static bool	**init_for_dp(char *str, char *pattern, int *str_len, int *ptn_len)
{
	bool	**dp;
	int		i;
	int		j;

	*str_len = ft_strlen(str);
	*ptn_len = ft_strlen(pattern);
	dp = ft_xcalloc(*str_len + 1, sizeof(bool *));
	i = -1;
	while (++i <= *str_len)
		dp[i] = ft_xcalloc(*ptn_len + 1, sizeof(bool));
	dp[0][0] = true;
	if (str[0] != '.')
	{
		j = 0;
		while (++j <= *ptn_len)
		{
			if (pattern[j - 1] == '*')
				dp[0][j] = dp[0][j - 1];
		}
	}
	return (dp);
}

static void	free_dp(bool **dp, int len[2])
{
	int	i;

	i = -1;
	while (++i <= len[0])
		free(dp[i]);
	free(dp);
}

bool	match_pattern(char *str, char *pattern)
{
	bool	**dp;
	int		i;
	int		j;
	int		len[2];
	bool	ret;

	dp = init_for_dp(str, pattern, &len[0], &len[1]);
	i = 0;
	while (++i <= len[STR])
	{
		j = 0;
		while (++j <= len[PATTERN])
		{
			if (pattern[j - 1] == '*' && !(str[i - 1] == '.' && i * j == 1))
				dp[i][j] = dp[i - 1][j] | dp[i][j - 1];
			else if (pattern[j - 1] == str[i - 1])
				dp[i][j] = dp[i - 1][j - 1];
			else
				dp[i][j] = false;
		}
	}
	ret = dp[len[STR]][len[PATTERN]];
	free_dp(dp, len);
	return (ret);
}
