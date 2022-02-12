#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
	char *line = NULL;
	int i=0;

	while (1)
	{
		i++;
		line = readline("> ");
		if (line == NULL || strlen(line) == 0)
		{
			free(line);
			break;
		}
		add_history(line);
		printf("line is '%s'\n", line);
		free(line);
	}
	printf("exit\n");
	return 0;
}
