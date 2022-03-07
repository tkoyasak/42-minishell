#include "execution.h"

void	set_heredoc(t_process *process, char *limiter)
{
	size_t	len;
	char	*temp;
	char	*new_document;
	int		res;

	new_document = ft_strdup("");
	if (!new_document)
		return ;
	len = ft_strlen(limiter);
	limiter[len] = '\n';
	res = get_next_line(STDIN, &temp);
	while (temp && ft_strncmp(temp, limiter, len + 1))
	{
		new_document = ft_strjoin(new_document, temp);
		if (!new_document)
		{
			// free_one(&temp);
			// exit(free_all(NULL, pdata, true));
		}
		// free_one(&(pdata->total_doc));
		// free_one(&temp);
		res = get_next_line(STDIN, &temp);
	}
	// free_one(&temp);
	process->heredoc = new_document;
}
