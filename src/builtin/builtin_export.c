#include "minishell.h"

void	builtin_export_print(t_list *env_list)
{
	t_list	*itr;

	itr = env_list->next;
	while (itr)
	{
		printf("declare -x %s=\"%s\"\n", ((t_env *)(itr->content))->key, ((t_env *)(itr->content))->val);
		itr = itr->next;
	}
}

static bool validate_args(char *arg)
{
	bool valid;

	valid = true;
	if (ft_strlen(arg) == 0)
		valid = false;
	if (valid)
	{
		while (ft_isalnum(*arg) || *arg == '_')
			arg++;
		if (*arg != '=')
			valid = false;
	}
	if (!valid)
	{
		ft_putstr_fd("bash: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	}
	return (valid);
}

bool	set_env_value(char *arg, t_list *env_list)
{
	t_list	*itr;
	char	*key;
	char	*val;
	t_env	*env;

	if (!validate_args(arg))
		return (false);
	key = ft_strndup(arg, ft_strchr(arg, '=') - arg);
	val = ft_strdup(ft_strchr(arg, '=') + 1);
	itr = env_list->next;
	while (itr)
	{
		if (ft_strcmp(((t_env *)(itr->content))->key, key) == 0)
		{
			free(((t_env *)(itr->content))->val);
			((t_env *)(itr->content))->val = val;
			return (true);
		}
		itr = itr->next;
	}
	env = ft_calloc(1, sizeof(t_env));
	env->key = key;
	env->val = val;
	ft_lstadd_back(&env_list, ft_lstnew(env));
	return (true);
}

int	builtin_export(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	char	*arg;

	(void)expression;
	if (ft_lstsize((t_list *)(process->token_list)) == 1)
		builtin_export_print(shell_var->env_list);
	else
	{
		arg = ((t_token *)(process->token_list->next->content))->str;
		if (!set_env_value(arg, shell_var->env_list))
			return (1);
	}
	return (0);
}
