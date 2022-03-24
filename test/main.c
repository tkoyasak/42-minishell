#include "minishell.h"

int	main(void)
{
	char	*str[] = {"ab.c", "abc", ".gitignore"};
	char	*pattern[] = {".*", "*.c", "a*c", "*ab*", "*ab*ab*"};

	int	i = -1;
	while (++i < 3)
	{
		printf("===== case: %s =====\n", str[i]);
		int	j = -1;
		while (++j < 5)
		{
			if (match_pattern(str[i], pattern[j]))
				printf(GREEN"ok:%s %s\n"RESET, str[i], pattern[j]);
			else
				printf(RED"\tng:%s %s\n"RESET, str[i], pattern[j]);
		}
	}
}
