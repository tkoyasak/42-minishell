#include "minishell.h"

char	*join_path_name(char *prefix, char **slash_splitted_str)
{
	char	*path_name;

	path_name = prefix;
	while (*slash_splitted_str)
	{
		path_name = ft_strjoin_free(path_name, *slash_splitted_str, false);
		slash_splitted_str++;
	}
	return (path_name);
}

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
	dp = opendir(path_name);
	if (dp == NULL)
	{
		expansion = ft_calloc(1, sizeof(t_expansion));
		expansion->str = join_path_name(path_name, slash_splitted_str);
		printf("94: %s\n", expansion->str);
		expansion->len = ft_strlen(expansion->str);
		return (ft_lstnew(expansion));
	}
	while ((dirp = readdir(dp)) != NULL)
	{
		// 一致するのを探す
		joined_path_name = ft_strjoin(path_name, dirp->d_name);
		// printf("79: %s\n", joined_path_name);
		tmp = get_opened_directory(joined_path_name, slash_splitted_str + 1);
		ft_lstadd_back(&head, tmp);
	}
	return (head);
}

// ./ppp/  /iii/pppp/*/ooooo    docs
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
	else if (*str == '.')
		head = get_opened_directory("", slash_splitted_str);
	else
		head = get_opened_directory("./", slash_splitted_str);
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
