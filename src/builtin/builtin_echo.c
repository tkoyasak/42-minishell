#include "minishell.h"

bool	is_valid_option(char *str)
{
	if (!str)
		return (false);
	if (*str != '-')
		return (false);
	str++;
	while (*str)
	{
		if (*str != 'n')
			return (false);
		str++;
	}
	return (true);
}

int	builtin_echo(t_process *process, t_shell_var *shell_var)
{
	t_list			*itr;
	bool			has_endl;

	has_endl = true;
	itr = process->token_list->next;
	if (itr == NULL)
	{
		printf("\n");
		return (0);
	}
	if (is_valid_option(((t_token *)(itr->content))->str))
		has_endl = false;
	while (is_valid_option(((t_token *)(itr->content))->str))
		itr = itr->next;
	while (itr)
	{
		printf("%s", ((t_token *)(itr->content))->str);
		itr = itr->next;
		if (itr)
			printf(" ");
	}
	if (has_endl)
		printf("\n");
	return (0);
}
