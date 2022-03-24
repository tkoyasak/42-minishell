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

bool	match_pattern(char *str, char *pattern)
{
	bool	**dp;
	size_t	i;
	size_t	j;
	size_t	str_len;
	size_t	ptn_len;

	str_len = ft_strlen(str);
	ptn_len = ft_strlen(pattern);
	dp = ft_xcalloc(str_len + 1, sizeof(bool *));
	i = -1;
	while (++i <= str_len)
		dp[i] = ft_xcalloc(ptn_len + 1, sizeof(bool));
	dp[0][0] = true;
	if (str[0] != '.')
	{
		j = 0;
		while (++j <= ptn_len)
			{
				if (pattern[j - 1] == '*')
					dp[0][j] = dp[0][j - 1];
			}
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

t_list	*matched_files(char *prefix, char **slash_splitted_strs)
{
	t_expd		*expansion;
	char			*path_name;
	char			*joined_path_name;
	DIR				*dp;
	struct dirent	*dirp;
	t_list			*tmp;
	t_list			*head;

	if (*slash_splitted_strs == NULL)
	{
		expansion = ft_xcalloc(1, sizeof(t_expd));
		expansion->str = prefix;
		expansion->len = ft_strlen(prefix);
		expansion->kind = PD_FILENAME;
		return (ft_xlstnew(expansion));
	}
	head = NULL;
	path_name = prefix;
	while (*slash_splitted_strs)
	{
		if (ft_strchr(*slash_splitted_strs, '*'))
			break ;
		path_name = ft_strjoin(path_name, *slash_splitted_strs);
		path_name = ft_strjoin(path_name, "/");
		slash_splitted_strs++;
	}
	char	*cwd_path = getcwd(NULL, 0);
	if (*path_name == '\0')
		dp = opendir(cwd_path);
	else
		dp = opendir(path_name);
	if (dp == NULL)
		return (NULL);
	while ((dirp = readdir(dp)) != NULL)
	{
		if (match_pattern(dirp->d_name, *(slash_splitted_strs)) == false)
			continue ;
		joined_path_name = ft_strjoin(path_name, dirp->d_name);
		if (*(slash_splitted_strs + 1) != NULL)
			joined_path_name = ft_strjoin(joined_path_name, "/");
		tmp = matched_files(joined_path_name, slash_splitted_strs + 1);
		ft_lstadd_back(&head, tmp);
	}
	closedir(dp);
	return (head);
}
