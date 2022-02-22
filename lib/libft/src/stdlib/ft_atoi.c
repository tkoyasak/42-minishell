/*
** Converts initial portion of string pointed to by 'str' to int representation.
** It is equivalent to:
**  (int)	strtol(str, (char **)NULL, 10);
**
** man strtol
**  long	strtol(const char *restrict str, char **restrict endptr, int base);
** The strtol() converts string in 'str' to 'long value'.
** The conversion is done according to the given base.
** The string may begin with arbitrary amount of white space (isspace(3))
** followed by a single optional `+' or `-' sign.
** The remainder of the string is converted to a long value in obvious manner,
** stopping at first character which is not valid digit in the given base.
** Returns values:
**  1) result of conversion, unless value would underflow or overflow.
**  2) 0, if no conversion could be performed.
**  3) LONG_MAX(overflow) or LONG_MIN(underflow), if overflow/underflow occurs.
*/

#include "libft.h"

int	ft_atoi(const char *str)
{
	long	num;
	int		sign;

	num = 0;
	sign = 1;
	while (ft_strchr("\t\n\v\f\r ", *str))
		str++;
	if (*str == '-')
		sign = -1;
	if (ft_strchr("+-", *str))
		str++;
	while (ft_isdigit(*str))
	{
		if ((num * 10 + (*str - '0')) / 10 != num)
		{
			if (sign == 1)
				return ((int)LONG_MAX);
			else if (sign == -1)
				return ((int)LONG_MIN);
		}
		num = num * 10 + (*str++ - '0');
	}
	return ((int)(num * sign));
}
