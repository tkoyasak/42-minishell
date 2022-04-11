NAME		:= minishell

LIBROOT		:= lib
LIBFTDIR	:= $(addprefix $(LIBROOT)/, libft)
LIBFT		:= libft.a
READLINEDIR := $(shell brew --prefix readline)

SRCROOT		:= src
SRCDIRS		:= $(shell find $(SRCROOT) -type d)
# SRCS		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
SRCS		:= src/utils/delete_content.c src/utils/delete_content2.c src/utils/error_handler.c src/utils/handle_signal.c src/utils/safe_func.c src/utils/wrapper.c src/utils/wrapper_str.c src/heredoc/expander_heredoc.c src/heredoc/set_heredoc.c src/env/get_env_value.c src/env/get_environ.c src/env/init_envlist.c src/env/init_sh_var.c src/env/set_env_value.c src/env/sort_env_list.c src/expander/convert_to_token_list.c src/expander/expand_token.c src/expander/expand_wildcard.c src/expander/expander.c src/expander/extract_word.c src/expander/match_files.c src/expander/match_pattern.c src/executor/exec_builtin.c src/executor/exec_child.c src/executor/exec_processes.c src/executor/exec_single_builtin.c src/executor/executor.c src/executor/get_cmd_path.c src/executor/handle_fd.c src/executor/set_command.c src/executor/set_io_params.c src/main/main.c src/builtin/builtin_cd.c src/builtin/builtin_cd2.c src/builtin/builtin_echo.c src/builtin/builtin_env.c src/builtin/builtin_exit.c src/builtin/builtin_export.c src/builtin/builtin_pwd.c src/builtin/builtin_unset.c src/analyzer/convert_to_expression_tree.c src/analyzer/lexer.c src/analyzer/node_utils.c src/analyzer/parser.c src/analyzer/validate_line.c

OBJROOT		:= obj
OBJDIRS		:= $(patsubst $(SRCROOT)/%, $(OBJROOT)/%, $(SRCDIRS))
OBJS		:= $(patsubst $(SRCROOT)/%, $(OBJROOT)/%, $(SRCS:.c=.o))

DEPS		:= $(OBJS:.o=.d)

INCDIRS		:= includes $(LIBFTDIR)/includes $(READLINEDIR)/include
INCLUDE		:= $(addprefix -I, $(INCDIRS))

CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -MMD -MP
LFLAGS		:= -L$(LIBFTDIR) -lft -L$(READLINEDIR)/lib -lreadline -lhistory

RM			:= rm -f

all: $(NAME)

$(NAME): $(LIBFTDIR)/$(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

$(OBJROOT)/%.o: $(SRCROOT)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFTDIR)/$(LIBFT):
	make -sC $(LIBFTDIR)

bonus: all

clean:
	make clean -sC $(LIBFTDIR)
	$(RM) -r $(OBJROOT)

fclean: clean
	make fclean -sC $(LIBFTDIR)
	$(RM) $(NAME)

re: fclean all

debug: CFLAGS += -g3 -fsanitize=address
debug: re

leak:
	while [ 1 ]; do leaks minishell; sleep 1; done

gtest:
	cmake -S ./test/gtest -B ./test/gtest/build && cmake --build ./test/gtest/build && ./test/gtest/build/main

shtest: $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDE) test/shtest/main.c $^ $(LFLAGS) -o test/shtest/shtest
	@chmod +x test/shtest/shtest
	@chmod +x test/shtest/executor.sh
	@cd test/shtest && ./executor.sh

test:
	./$(NAME) < ./test/test.sh

asterisk: $(LIBFTDIR)/$(LIBFT) $(OBJS)
	gcc $(INCLUDE) test/main.c obj/expander/match_pattern.o lib/libft.a -o test/asterisk
	./test/asterisk

-include $(DEPS)

.PHONY: all clean fclean re test

echo:
	@echo $(SRCS) >> Makefile
