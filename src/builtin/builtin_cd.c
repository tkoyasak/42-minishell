#include "minishell.h"

int		builtin_cd(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	char	*path_name;
	char	*relative_path;
	char	*tmp_path;

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
		free(path_name);
		return (1);
	}
	// shell_var->pwd = getcwd(NULL, 0);
	if (getcwd(NULL, 0) == NULL)
	{
		ft_putendl_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory", STDERR_FILENO);
		relative_path = ft_strjoin("/", path_name);
		shell_var->pwd = ft_strjoin_free(shell_var->pwd, relative_path, true);
	}
	else
		shell_var->pwd = getcwd(NULL, 0);
	free(path_name);
	return (0);
}
