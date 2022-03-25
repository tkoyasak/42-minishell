/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 12:05:45 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 12:07:38 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*create_pd_filename(char *prefix)
{
	t_expd	*expd;

	expd = ft_xcalloc(1, sizeof(t_expd));
	expd->str = ft_xstrdup(prefix);
	expd->len = ft_strlen(prefix);
	expd->kind = PD_FILENAME;
	return (ft_xlstnew(expd));
}

static DIR	*search_directory(char **path_name, char **slash_splitted_strs)
{
	DIR		*dp;
	char	*cwd_path;

	while (*slash_splitted_strs)
	{
		if (ft_strchr(*slash_splitted_strs, '*'))
			break ;
		*path_name = ft_xstrjoin_free(*path_name, *slash_splitted_strs, false);
		*path_name = ft_xstrjoin_free(*path_name, "/", false);
		slash_splitted_strs++;
	}
	cwd_path = getcwd(NULL, 0);
	if (**path_name == '\0')
		dp = opendir(cwd_path);
	else
		dp = opendir(*path_name);
	free(cwd_path);
	return (dp);
}

static void	add_back_file_list(t_list **head, char *path_name, char *dir_name, \
													char **slash_splitted_strs)
{
	char	*joined_path_name;
	t_list	*new_file_list;

	joined_path_name = ft_xstrjoin(path_name, dir_name);
	if (*(slash_splitted_strs + 1) != NULL)
		joined_path_name = ft_xstrjoin_free(joined_path_name, "/", false);
	new_file_list = matched_files(joined_path_name, slash_splitted_strs + 1);
	free(joined_path_name);
	ft_lstadd_back(head, new_file_list);
}

t_list	*matched_files(char *prefix, char **slash_splitted_strs)
{
	char			*path_name;
	DIR				*dp;
	struct dirent	*dirp;
	t_list			*head;
	char			*dir_name;

	if (*slash_splitted_strs == NULL)
		return (create_pd_filename(prefix));
	path_name = ft_xstrdup(prefix);
	dp = search_directory(&path_name, slash_splitted_strs);
	if (dp == NULL)
		return (free(path_name), NULL);
	head = NULL;
	dirp = readdir(dp);
	while (dirp != NULL)
	{
		dir_name = dirp->d_name;
		dirp = readdir(dp);
		if (match_pattern(dir_name, *(slash_splitted_strs)) == false)
			continue ;
		add_back_file_list(&head, path_name, dir_name, slash_splitted_strs);
	}
	free(path_name);
	closedir(dp);
	return (head);
}
