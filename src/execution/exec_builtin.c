#include "minishell.h"

bool	is_builtin(char *cmd)
{
	const char	*builtin_str[] = \
			{"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int			idx;

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
	const char	*builtin_str[] = \
			{"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	static	int	(*builtin_func[])(t_process *, t_shell_var *) = \
			{&builtin_echo, &builtin_cd, &builtin_pwd, &builtin_export, \
			&builtin_unset, &builtin_env, &builtin_exit};
	char		*cmd;
	int			idx;

	cmd = process->command[0];
	idx = 0;
	while (idx < 7)
	{
		if (strcmp(cmd, builtin_str[idx]) == 0)
			return ((*builtin_func[idx])(process, shell_var));
		idx++;
	}
	return (1);
}
