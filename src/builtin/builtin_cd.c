#include "minishell.h"

int	builtin_cd_pwd_update(char *path_name, t_shell_var *shell_var)
{
	char	*relative_path;

	shell_var->oldpwd = shell_var->pwd;
	if (getcwd(NULL, 0) == NULL)
	{
		ft_putendl_fd("cd: error retrieving current directory: \
			getcwd: cannot access parent directories: \
			No such file or directory", STDERR_FILENO);
		relative_path = ft_strjoin("/", path_name);
		shell_var->pwd = ft_strjoin(shell_var->pwd, relative_path);
		free(relative_path);
	}
	else
		shell_var->pwd = getcwd(NULL, 0);
	free(path_name);
	set_env_value("OLDPWD", shell_var->oldpwd, shell_var);
	set_env_value("PWD", shell_var->pwd, shell_var);
	return (0);
}

int	builtin_cd(t_proc *proc, t_shell_var *shell_var)
{
	char	*path_name;

	path_name = proc->command[1];
	if (path_name == NULL)
	{
		path_name = get_env_value("HOME", shell_var);
		if (path_name == NULL)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (1);
		}
		if (*path_name == '\0')
		{
			return (0);
		}
	}
	if (chdir(path_name) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path_name, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		free(path_name);
		return (1);
	}
	return (builtin_cd_pwd_update(path_name, shell_var));
}
