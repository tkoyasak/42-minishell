#include "libft.h"

/*
** Return Value:
** - 1  : have a line and line's length is one or more.
** - 0  : reach end of file and line's length is zero.
** - -1 : an error occur. line is set to NULL.
*/

static int	set_return_value(char *ptr_newline, char **memo, char **line)
{
	char	*tmp;

	if (ptr_newline)
	{
		*line = ft_strndup(*memo, ptr_newline - *memo);
		if (!*line)
			return (ERROR);
		tmp = ft_strndup(ptr_newline + 1, ft_strlen(ptr_newline + 1));
		if (!tmp)
			free(*line);
		if (!tmp)
			return (ERROR);
		free(*memo);
		*memo = tmp;
		return (SUCCESS);
	}
	*line = ft_strdup(*memo);
	if (!*line)
		return (ERROR);
	free(*memo);
	*memo = NULL;
	if (**line == '\0')
		return (END_OF_FILE);
	return (SUCCESS);
}

static int	read_fd(int fd, char **memo, char **buf, char **line)
{
	ssize_t	read_byte;
	char	*ptr_newline;
	char	*tmp;

	tmp = NULL;
	while (1)
	{
		ptr_newline = ft_strchr(*memo, '\n');
		if (ptr_newline)
			break ;
		read_byte = read(fd, *buf, BUFFER_SIZE);
		if (read_byte == -1)
			free(tmp);
		if (read_byte == -1)
			return (ERROR);
		if (read_byte == 0)
			break ;
		(*buf)[read_byte] = '\0';
		tmp = ft_strjoin(*memo, *buf);
		if (!tmp)
			return (ERROR);
		free(*memo);
		*memo = tmp;
	}
	return (set_return_value(ptr_newline, memo, line));
}

int	get_next_line(int fd, char **line)
{
	static char		*memo[OPEN_MAX];
	char			*buf;
	int				status;

	if (fd < 0 || OPEN_MAX <= fd || BUFFER_SIZE <= 0)
		return (ERROR);
	if (!memo[fd])
		memo[fd] = ft_strndup("", 0);
	if (!memo[fd])
		return (ERROR);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (ERROR);
	status = read_fd(fd, &memo[fd], &buf, line);
	free(buf);
	if (status == ERROR || status == END_OF_FILE)
	{
		free(memo[fd]);
		memo[fd] = NULL;
	}
	if (status == ERROR)
		*line = NULL;
	return (status);
}
