#include "minishell.h"

int		builtin_cd(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	char	*path_name;

	path_name = process->command[1];
	if (path_name == NULL)
	{
		path_name = get_env_value("HOME", shell_var);
	}
	if (chdir(path_name) == -1)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(path_name, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}

	char	*buff = getcwd(NULL, 0);
	printf("%s\n", buff);
	free(buff);
	return (0);
}
