/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:06:36 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:35:51 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status;

void	init_sh_var(t_sh_var *sh_var)
{
	sh_var->env_list = init_envlist();
	sh_var->pwd = getcwd(NULL, 0);
	sh_var->oldpwd = NULL;
}

int	analyzer(char *line, t_node **tree, t_sh_var *sh_var)
{
	t_list	*token_list;

	*tree = NULL;
	if (lexer(line, &token_list) == 1)
	{
		g_exit_status = 258;
		return (1);
	}
	if (parser(tree, token_list) == 1)
	{
		g_exit_status = 258;
		return (1);
	}
	*tree = convert_to_expr_tree(*tree);
	if (set_heredoc(*tree, sh_var) == 1)
	{
		g_exit_status = 1;
		return (1);
	}
	return (0);
}

void	minish_loop(t_sh_var *sh_var)
{
	char		*line;
	t_node		*tree;

	while (1)
	{
		line = readline(PROMPT);
		if (line == NULL)
		{
			printf("exit\n");
			line = ft_xstrdup("exit");
		}
		if (ft_strlen(line))
		{
			add_history(line);
			if (analyzer(line, &tree, sh_var) == 0)
				g_exit_status = execution(tree, sh_var);
			delete_astree(tree);
		}
		detect_leak(__LINE__, __FILE__);
		// printf("g_exit_status:%d\n", g_exit_status);
		free(line);
	}
}

void	test_one_line(char *line, t_sh_var *sh_var)
{
	t_node		*tree;

	if (ft_strlen(line) == 0)
		exit(0);
	if (analyzer(line, &tree, sh_var) == 0)
		g_exit_status = execution(tree, sh_var);
	delete_astree(tree);
	ft_lstclear(&sh_var->env_list, delete_env);
	free(sh_var->pwd);
	free(sh_var->oldpwd);
}

int	main(int argc, char **argv)
{
	t_sh_var	sh_var;

	if (signal(SIGINT, sigint_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (1);
	init_sh_var(&sh_var);
	if (argc == 3 && !ft_strcmp("-c", argv[1]))
		test_one_line(argv[2], &sh_var);
	else
		minish_loop(&sh_var);
	return (g_exit_status);
}
