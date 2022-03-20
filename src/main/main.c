#include "minishell.h"

volatile sig_atomic_t	g_exit_status;

void	init_shell_var(t_shell_var *shell_var)
{
	shell_var->env_list = init_envlist();
	shell_var->pwd = getcwd(NULL, 0);
	shell_var->oldpwd = NULL;
}

int	analyzer(char *line, t_node **tree, t_shell_var *shell_var)
{
	t_list	*token_list;

	if (lexer(line, &token_list) == 1)
		return (1);
	if (parser(tree, token_list) == 1)
		return (1);
	*tree = convert_to_expression_tree(*tree);
	set_heredoc(*tree, shell_var);
	return (0);
}

void minish_loop(t_shell_var *shell_var)
{
	char		*line;
	t_node		*tree;

	while (1)
	{
		line = readline(PROMPT);
		if (line == NULL)
			exit(g_exit_status);
		if (ft_strlen(line))
		{
			add_history(line);
			if (!analyzer(line, &tree, shell_var))
				g_exit_status = execution(tree, shell_var);
			// clear tree
		}
		// printf("g_exit_status:%d\n", g_exit_status);
		free(line);
	}
}

void	test_one_line(t_shell_var *shell_var, int argc, char *argv[])
{
	t_node		*tree;

	if (!analyzer(argv[2], &tree, shell_var))
		g_exit_status = execution(tree, shell_var);
	// delete tree
	// analyzer(argv[2], &tree, shell_var);
	// delete_astree(tree);
	exit(g_exit_status);
}

int	main(int argc, char **argv)
{
	t_shell_var	shell_var;

	if (signal(SIGINT, sigint_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (1);
	init_shell_var(&shell_var);
	if (argc == 3 && !ft_strcmp("-c", argv[1]))
		test_one_line(&shell_var, argc, argv);
	else
		minish_loop(&shell_var);
	// delete shell_var
	return (0);
}
