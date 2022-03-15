#include "minishell.h"

volatile sig_atomic_t	g_exit_status;

void	sigint_handler(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 1;
}

void	init_shell_var(t_shell_var *shell_var)
{
	shell_var->env_list = init_envlist();
	shell_var->pwd = NULL;
	shell_var->oldpwd = NULL;
}

void minish_loop(t_shell_var *shell_var)
{
	char		*line;
	t_list		*token_list;
	t_node		*astree;
	int 		error_status;

	(void)token_list;
	while (1)
	{
		line = readline(CYAN"minish$ "RESET);
		while (line == NULL)
			line = readline("");
		if (ft_strlen(line) == 0)
			continue ;
		else
		{
			// token_list = lexer(line);
			// astree = parser(line);
			astree = expansion(line, shell_var);
			error_status = execution(astree, shell_var);
			printf("error_status: %d\n", error_status);
		}
		add_history(line);
		free(line);
	}
}

int	main(void)
{
	t_shell_var	shell_var;

	if (signal(SIGINT, sigint_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (1);
	init_shell_var(&shell_var);
	minish_loop(&shell_var);
	return (0);
}
