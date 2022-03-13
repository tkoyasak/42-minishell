NAME		:= libft.a

SRCROOT		:= src
SRCDIRS		:= $(shell find $(SRCROOT) -type d)
# SRCS		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
SRCS		:= src/ctype/ft_isalnum.c src/ctype/ft_isalpha.c src/ctype/ft_isascii.c src/ctype/ft_isdigit.c src/ctype/ft_isprint.c src/ctype/ft_isspace.c src/ctype/ft_tolower.c src/ctype/ft_toupper.c src/io/ft_putchar_fd.c src/io/ft_putnchar_fd.c src/io/ft_putendl_fd.c src/io/ft_putnbr_fd.c src/io/ft_putstr_fd.c src/io/get_next_line.c src/list/ft_lstadd_back.c src/list/ft_lstadd_front.c src/list/ft_lstclear.c src/list/ft_lstdelone.c src/list/ft_lstiter.c src/list/ft_lstlast.c src/list/ft_lstmap.c src/list/ft_lstnew.c src/list/ft_lstsize.c src/stdlib/ft_atoi.c src/stdlib/ft_calloc.c src/stdlib/ft_itoa.c src/str/ft_split.c  src/str/ft_strchr.c src/str/ft_strcmp.c src/str/ft_strdup.c src/str/ft_striteri.c src/str/ft_strjoin.c src/str/ft_strlcat.c src/str/ft_strlcpy.c src/str/ft_strlen.c src/str/ft_strmapi.c src/str/ft_strncmp.c src/str/ft_strndup.c src/str/ft_strnstr.c src/str/ft_strrchr.c src/str/ft_strtrim.c src/str/ft_substr.c src/mem/ft_bzero.c src/mem/ft_memchr.c src/mem/ft_memcmp.c src/mem/ft_memcpy.c src/mem/ft_memmove.c src/mem/ft_memset.c

OBJROOT		:= obj
OBJDIRS		:= $(subst $(SRCROOT), $(OBJROOT), $(SRCDIRS))
OBJS		:= $(subst $(SRCROOT), $(OBJROOT), $(SRCS:.c=.o))

DEPS		:= $(OBJS:.o=.d)

CC			:= gcc -O2
CFLAGS		:= -Wall -Wextra -Werror -MMD -MP
INCLUDE		:= -Iincludes

AR			:= ar
ARFLAGS		:= crs

RM			:= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJROOT)/%.o: $(SRCROOT)/%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) -r $(OBJROOT)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)

.PHONY : all clean fclean re
