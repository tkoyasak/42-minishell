#include "execution.h"
#include "builtin.h"

// int (*builtin_func[]) (char **) = {
//   &lsh_cd,
//   &lsh_help,
//   &lsh_exit,
//   &lsh_echo,
//   &lsh_pwd,
//   &lsh_export,
//   &lsh_unset,
//   &lsh_env
// };

// int lsh_execute(char **args)
// {
//   int i;

//   if (args[0] == NULL) {
//     // An empty command was entered.
//     return 1;
//   }

//   for (i = 0; i < lsh_num_builtins(); i++) {
//     if (strcmp(args[0], builtin_str[i]) == 0) {
//       return (*builtin_func[i])(args);
//     }
//   }

//   return lsh_launch(args);
// }

// // void	evaluate_builtin()
bool	exec_builtin(t_expression *expression, t_process *process, int cmd_idx, char *cmd)
{
	const char *builtin_str[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	int (*builtin_func[]) (t_expression *, t_process *, int) = \
			{&builtin_echo, &builtin_cd, &builtin_pwd, &builtin_export, \
			&builtin_unset, &builtin_env, &builtin_exit};
	int i;

	i = 0;
	while (i < 7)
	{
		if (strcmp(cmd, builtin_str[i]) == 0)
			exit((*builtin_func[i])(expression, process, cmd_idx));
		i++;
	}
	return (false);
}
