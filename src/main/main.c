/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:06:36 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/10 19:41:11 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status;

static int	analyzer(char *line, t_node **tree, t_sh_var *sh_var)
{
	t_list	*token_list;

	*tree = NULL;
	while (*line && ft_isspace(*line))
		line++;
	if (*line == '\0')
		return (1);
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

static void	minish_loop(t_sh_var *sh_var)
{
	char		*line;
	t_node		*tree;

	rl_outstream = stderr;
	while (1)
	{
		install_signal_handle();
		line = readline(PROMPT);
		rl_signal_event_hook = NULL;
		if (line == NULL)
			line = ft_xstrdup("exit");
		if (ft_strlen(line))
		{
			add_history(line);
			if (analyzer(line, &tree, sh_var) == 0)
				executor(tree, sh_var);
			delete_astree(tree);
		}
		free(line);
	}
	ft_lstclear(&sh_var->env_list, delete_env);
	free(sh_var->pwd);
	free(sh_var->oldpwd);
}

static void	test_one_line(char *line, t_sh_var *sh_var)
{
	t_node		*tree;

	if (ft_strlen(line) == 0)
		exit(0);
	if (analyzer(line, &tree, sh_var) == 0)
		executor(tree, sh_var);
	delete_astree(tree);
	ft_lstclear(&sh_var->env_list, delete_env);
	free(sh_var->pwd);
	free(sh_var->oldpwd);
}

int	main(int argc, char **argv)
{
	t_sh_var	sh_var;

	init_sh_var(&sh_var);
	if (argc == 3 && !ft_strcmp("-c", argv[1]))
		test_one_line(argv[2], &sh_var);
	else
		minish_loop(&sh_var);
	return (g_exit_status);
}
