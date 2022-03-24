#include "minishell.h"

ssize_t	safe_func(ssize_t res)
{
	if (res == -1)
		exit(EXIT_FAILURE);
	return (res);
}

void	detect_leak(int line, char *file) //削除
{
	char	buf[100];
	int		status;

	fprintf(stderr, "%s:%d\n", file, line);
	snprintf(buf, 50, "leaks -q %d &> leaks.out", getpid());
	fprintf(stderr, "calling leaks command...\n");
	status = system(buf);
	if (!status)
	{
		fprintf(stderr, GREEN"OK! No leak detected.\n"RESET);
	}
	else
	{
		fprintf(stderr, RED"KO! LEAK DETECTED!\n"RESET);
		system("cat leaks.out > /dev/stderr");
	}
}
