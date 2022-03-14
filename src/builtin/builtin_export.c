#include "minishell.h"

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

int	builtin_export(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	t_list	*env_list = init_envlist();
	t_list	*itr;
	char	*arg;
	char	*key;
	char	*val;
	t_env	*env;

	if (ft_lstsize((t_list *)(process->token_list)) == 1)
	{
		itr = env_list;
		while (itr)
		{
			printf("declare -x %s=\"%s\"\n", ((t_env *)(itr->content))->key, ((t_env *)(itr->content))->val);
			// printf("%s", ((t_env *)(itr->content))->key);
			// printf("=\"%s\"\n", ((t_env *)(itr->content))->val);
			itr = itr->next;
		}
	}
	else
	{
		arg = ((t_token *)(process->token_list->next->content))->str;
		// 引数のバリデイト
		if (!validate_args(arg))
			return (1);
		// 引数のtoken_listからkeyとvalを取り出す
		key = ft_strndup(arg, ft_strchr(arg, '=') - arg);
		val = ft_strdup(ft_strchr(arg, '=') + 1);
		// env_listにkeyと一致するものがあったら，そのkeyにvalを上書きする
		itr = env_list;
		while (itr)
		{
			if (ft_strcmp(((t_env *)(itr->content))->key, key) == 0)
			{
				free(((t_env *)(itr->content))->val);
				((t_env *)(itr->content))->val = val;
				break ;
			}
			itr = itr->next;
		}
		// なければ，新しくkeyとvalを追加する
		if (itr == NULL)
		{
			env = ft_calloc(1, sizeof(t_env));
			env->key = key;
			env->val = val;
			ft_lstadd_back(&env_list, ft_lstnew(env));
		}
	}
	// printf("called builtin_export\n");
	return (0);
}
