#include "minishell.h"

t_list	*get_opened_directory(char *prefix, char **slash_splitted_str)
{
	t_expansion		*expansion;
	char			*path_name;
	char			*joined_path_name;
	DIR				*dp;
	struct dirent	*dirp;
	t_list			*tmp;
	t_list			*head;

	if (*slash_splitted_str == NULL)
	{
		expansion = ft_calloc(1, sizeof(t_expansion));
		expansion->str = prefix;
		expansion->len = ft_strlen(prefix);
		expansion->kind = FILENAME_EXPANSION;
		return (ft_lstnew(expansion));
	}
	head = NULL;
	path_name = prefix;
	while (*slash_splitted_str)
	{
		if (ft_strchr(*slash_splitted_str, '*'))
			break ;
		path_name = ft_strjoin(path_name, *slash_splitted_str);
		path_name = ft_strjoin(path_name, "/");
		slash_splitted_str++;
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
		if (match_given_pattern(dirp->d_name, *(slash_splitted_str)) == false)
			continue ;
		joined_path_name = ft_strjoin(path_name, dirp->d_name);
		if (*(slash_splitted_str + 1) != NULL)
			joined_path_name = ft_strjoin(joined_path_name, "/");
		tmp = get_opened_directory(joined_path_name, slash_splitted_str + 1);
		ft_lstadd_back(&head, tmp);
	}
	closedir(dp);
	return (head);
}

t_list	*get_expanded_filename_token(t_list *expansion_list)
{
	t_expansion	*expansion;
	t_list		*head; // expansion_list_itr
	char		*str;
	char		**slash_splitted_str;

	expansion = expansion_list->content;
	str = expansion->str;
	if (ft_strchr(str, '*') == NULL)
		return (expansion_list);
	slash_splitted_str = ft_split(str, '/');
	if (*str == '/')
		head = get_opened_directory("/", slash_splitted_str);
	else
		head = get_opened_directory("", slash_splitted_str);
	if (head == NULL)
		return (expansion_list);
	return (head);
}

/*   t_expansion_kindがSTRINGでかつin_squoteとin_dquoteがfalseで、
*を含むものを展開   */
t_list	*get_filename_expansion(t_list *expansion_list)
{
	t_list	head;
	t_list	*itr; //expansion_list_itr
	t_list	*next;
	t_list	*prev;
	bool	is_delimiter;

	head.next = NULL;
	prev = &head;
	itr = expansion_list;
	is_delimiter = false;
	while (itr)
	{
		next = itr->next;
		if (!is_delimiter && ((t_expansion *)(itr->content))->kind == STRING && \
			((t_expansion *)(itr->content))->in_dquote == false && \
			((t_expansion *)(itr->content))->in_squote == false )
			itr = get_expanded_filename_token(itr);
		prev->next = itr;
		while (itr->next != NULL && itr->next != next)
			itr = itr->next;
		is_delimiter = false;
		if (((t_expansion *)(itr->content))->kind == TK_REDIRECT && ft_strcmp(((t_expansion *)(itr->content))->str, "<<") == 0)
			is_delimiter = true;
		itr->next = next;
		prev = itr;
		itr = itr->next;
	}
	return (head.next);
}
