#include "minishell.h"

bool	is_builtin(char *cmd)
{
	const char *builtin_str[] = \
			{"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int idx;

	idx = 0;
	while (idx < 7)
	{
		if (ft_strcmp(cmd, builtin_str[idx]) == 0)
			return (true);
		idx++;
	}
	return (false);
}

/*  return exit status  */
int	exec_builtin(t_process *process, t_shell_var *shell_var)
{
	const char *builtin_str[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int (*builtin_func[]) (t_process *, t_shell_var *) = \
			{&builtin_echo, &builtin_cd, &builtin_pwd, &builtin_export, \
			&builtin_unset, &builtin_env, &builtin_exit};
	char	*cmd;
	int 	i;

	cmd = process->command[0];
	i = 0;
	while (i < 7)
	{
		if (strcmp(cmd, builtin_str[i]) == 0)
			return ((*builtin_func[i])(process, shell_var));
		i++;
	}
	return (1);
}
