NAME		:= a.out

LIBROOT		:= lib
LIBFTDIR	:= $(addprefix $(LIBROOT)/, libft)
LIBFT		:= libft.a

SRCROOT		:= src
SRCDIRS		:= $(shell find $(SRCROOT) -type d)
SRCS		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

OBJROOT		:= obj
OBJDIRS		:= $(subst $(SRCROOT), $(OBJROOT), $(SRCDIRS))
OBJS		:= $(subst $(SRCROOT), $(OBJROOT), $(SRCS:.c=.o))

DEPS		:= $(OBJS:.o=.d)

CC			:= gcc
# CFLAGS		:= -Wall -Wextra -Werror -MMD -MP
CFLAGS		:= -MMD -MP
LFLAGS		:= -L$(LIBFTDIR) -lft
INCLUDE		:= -Iincludes -I$(LIBFTDIR)/includes

RM			:= rm -f


all: $(NAME)

$(NAME): $(LIBFTDIR)/$(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

$(OBJROOT)/%.o: $(SRCROOT)/%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFTDIR)/$(LIBFT):
	make -sC $(LIBFTDIR)

clean:
	make clean -sC $(LIBFTDIR)
	$(RM) -r $(OBJROOT)

fclean: clean
	make fclean -sC $(LIBFTDIR)
	$(RM) $(NAME)

re: fclean all

gtest:
	cmake -S ./test -B ./test/build && cmake --build ./test/build && ./test/build/main

-include $(DEPS)

.PHONY: all clean fclean re