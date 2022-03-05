#include "execution.h"

static void	free_str(char **str)
{
	if (str && (*str))
	{
		free(*str);
		*str = NULL;
	}
}

static void	cmd_not_found(char *cmd)
{
	ft_putstr_fd("command not found: ", STDERR);
	ft_putendl_fd(cmd, STDERR);
}

static char	*get_fullcmd_core(char *cmd, char **all_paths)
{
	int		path_index;
	char	*fullcmd;
	char	*temp;

	path_index = -1;
	while (all_paths[++path_index])
	{
		temp = ft_strjoin(all_paths[path_index], "/");
		fullcmd = ft_strjoin(temp, cmd);
		free_str(&temp);
		if (!(fullcmd))
			exit(EXIT_FAILURE);
		if (!(access(fullcmd, X_OK)))
			return (fullcmd);
		free_str(&fullcmd);
	}
	cmd_not_found(cmd);
	return (NULL);
}

/*  return (NULL) if fail  */
char	*get_fullpath_cmd(char *cmd)
{
	char	**all_paths;
	char	*path_env;

	if (!cmd)
		exit(EXIT_FAILURE);
	if (!access(cmd, X_OK))
		return (cmd);
	path_env = getenv("PATH");
	if (!path_env)
	{
		cmd_not_found(cmd);
		return (NULL);
	}
	all_paths = ft_split(&(path_env[PATH_PREFIX]), ':');
	if (!all_paths)
		exit(EXIT_FAILURE);
	return (get_fullcmd_core(cmd, all_paths));
}
