/* 動かない */

/*  execute child process  */
void	exec_child(t_pdata *pdata, const int cmd_index)
{
	int		filefd;
	char	**cmd;
	char	*fullpath_cmd;

	if (pdata->has_heredoc && cmd_index == 0)
		exec_child_heredoc(pdata);
	filefd = get_filefd(pdata, cmd_index);
	if (filefd == -1)
	{
		perror(pdata->file);
		exit(EXIT_FAILURE);
	}
	cmd = get_cmd(pdata, cmd_index);
	fullpath_cmd = get_fullpath_cmd(pdata, cmd);
	dup2_func(pdata, filefd, cmd_index);
	close_func(pdata, filefd, cmd_index);
	execve(fullpath_cmd, cmd, pdata->envp);
	exit(NOCMD);
}


/*   execute processes  */
int	exec_processes(t_info *info, t_pdata *pdata)
{
	int	wstatus;
	int	cmd_index;

	cmd_index = 0;
	while (cmd_index < pdata->process_cnt)
	{
		set_file(info, pdata, cmd_index);
		if (cmd_index < pdata->process_cnt - 1)
			create_pipe(pdata, cmd_index);
		info->pid[cmd_index] = fork();
		if (info->pid[cmd_index] == -1)
			exit(free_all(info, pdata, true));
		else if (info->pid[cmd_index] == 0)
			exec_child(pdata, cmd_index);
		else if (cmd_index)
		{
			safe_func(close(pdata->pipefd[cmd_index - 1][PIPEIN]));
			safe_func(close(pdata->pipefd[cmd_index - 1][PIPEOUT]));
		}
		cmd_index++;
	}
	wstatus = wait_all_process(info, pdata);
	return (WEXITSTATUS(wstatus));
}

static void	prepare_pipe(t_info *info, t_pdata *pdata)
{
	int	pipe_index;

	pdata->pipefd = (int **)malloc(sizeof(int *) * (pdata->process_cnt));
	if (!pdata->pipefd)
		exit(free_all(info, pdata, true));
	pipe_index = -1;
	while (++pipe_index < pdata->process_cnt)
		pdata->pipefd[pipe_index] = NULL;
}

void	pipex(int argc, char **argv, char **envp)
{
	t_info		info;
	t_pdata		pdata;
	int			error_status;

	init_pdata(&pdata, argc, argv, envp);
	init_info(&info, &pdata, argc, pdata.has_heredoc);
	prepare_pipe(&info, &pdata);
	error_status = exec_processes(&info, &pdata);
	free_all(&info, &pdata, false);
	exit(error_status);
}
