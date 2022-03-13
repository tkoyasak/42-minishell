#include "minishell.h"

volatile sig_atomic_t	g_exit_states;

void	sigint_handler(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_states = 1;
}


void	init_shell_var(t_shell_var *shell_var)
{
	shell_var->env_list = init_envlist();
	shell_var->pwd = NULL;
	shell_var->oldpwd = NULL;
}

void minish_loop(void)
{
	char		*line;
	t_list		*token_list;
	t_node		*astree;
	t_shell_var	shell_var;
	int 		error_status;

	init_shell_var(&shell_var);
	while (1)
	{
		line = readline("minish$ ");
		if (line == NULL || ft_strlen(line) == 0)
		{
			perror("readline");
		}
		else
		{
			// token_list = lexer(line);
			// astree = parser(line);
			astree = expansion(line);
			error_status = execution(astree, &shell_var);
			printf("error_status: %d\n", error_status);
		}
		add_history(line);
		free(line);
	}
}

int	main(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	minish_loop();
	return (0);
}