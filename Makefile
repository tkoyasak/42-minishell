NAME		:= minishell

LIBROOT		:= lib
LIBFTDIR	:= $(addprefix $(LIBROOT)/, libft)
LIBFT		:= libft.a
READLINEDIR := $(shell brew --prefix readline)

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
LFLAGS		:= -L$(LIBFTDIR) -lft -L$(READLINEDIR)/lib -lreadline -lhistory
INCLUDE		:= -Iincludes -I$(LIBFTDIR)/includes -I$(READLINEDIR)/include


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
	cmake -S ./test/gtest -B ./test/gtest/build && cmake --build ./test/gtest/build && ./test/gtest/build/main

shtest: $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDE) test/shtest/main.c $^ $(LFLAGS) -o test/shtest/shtest
	@chmod +x test/shtest/shtest
	@chmod +x test/shtest/execution.sh
	@cd test/shtest && ./execution.sh

asterisk: $(LIBFTDIR)/$(LIBFT) $(OBJS)
	gcc $(INCLUDE) test/main.c obj/expansion/match_given_pattern.o lib/libft.a -o test/asterisk
	./test/asterisk
-include $(DEPS)

.PHONY: all clean fclean re