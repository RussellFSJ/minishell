NAME        := minishell

CC          := cc
CFLAGS      := -Wall -Wextra -Werror
DEPFLAGS    := -MMD -MP

SRC_DIR     := src
BUILD_DIR    := build
TEST_DIR    := tests
INC_DIR     := include
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a

INCLUDES    := -I$(INC_DIR) -I$(LIBFT_DIR)/include
LDLIBS      := -lreadline

# readline: if the link fails on undefined tputs/tgetent, the cluster's
# libreadline is not linked against terminfo - add -lncurses here.

# one variable per module, so the test targets can reuse the sets
ENV_SRC     := env/env_init.c env/env_get.c env/env_array.c env/env_utils.c
LEX_SRC     := lexer/lexer.c lexer/lexer_quotes.c lexer/lexer_operators.c \
               lexer/lexer_dump.c
BLT_SRC     := builtins/builtin_dispatch.c builtins/builtin_echo_pwd.c \
               builtins/builtin_cd.c builtins/builtin_export.c \
               builtins/builtin_unset_env.c builtins/builtin_exit.c
CLN_SRC     := cleanup/free_tokens.c cleanup/free_commands.c \
               cleanup/free_env.c
PRS_SRC 	:= parser/parser.c parser/parser_args.c parser/parser_redirs.c \
				parser/parser_syntax.c
EXP_SRC     := expander/expander.c expander/expander_variable.c \
               expander/expander_quotes.c
EXE_SRC     := executor/executor.c executor/executor_pipes.c \
               executor/executor_path.c executor/executor_child.c \
               executor/executor_status.c
RDR_SRC     := redirect/redirect.c redirect/heredoc.c
SIG_SRC     := signals/signals.c signals/signals_heredoc.c

SRCS        := main.c $(ENV_SRC) $(LEX_SRC) $(BLT_SRC) $(CLN_SRC) \
               $(PRS_SRC) $(EXP_SRC) $(EXE_SRC) $(RDR_SRC) $(SIG_SRC)

OBJS        := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

ENV_OBJ     := $(ENV_SRC:%.c=$(BUILD_DIR)/%.o)
LEX_OBJ     := $(LEX_SRC:%.c=$(BUILD_DIR)/%.o)
BLT_OBJ     := $(BLT_SRC:%.c=$(BUILD_DIR)/%.o)
CLN_OBJ     := $(CLN_SRC:%.c=$(BUILD_DIR)/%.o)
PRS_OBJ     := $(PRS_SRC:%.c=$(BUILD_DIR)/%.o)
EXE_OBJ     := $(EXE_SRC:%.c=$(BUILD_DIR)/%.o) \
               $(RDR_SRC:%.c=$(BUILD_DIR)/%.o) \
               $(SIG_SRC:%.c=$(BUILD_DIR)/%.o)

TESTS       := test_env test_lexer test_builtin test_parser test_exec

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDLIBS) -o $(NAME)

# libft is frozen at M0, so a plain file target is correct here: it is built
# once and `make` twice relinks nothing. Touching libft means `make re`.
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

tests: $(TESTS)

test_env: $(TEST_DIR)/test_env.c $(LIBFT) $(ENV_OBJ) $(CLN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(ENV_OBJ) $(CLN_OBJ) $(LIBFT) -o $@

test_lexer: $(TEST_DIR)/test_lexer.c $(LIBFT) $(LEX_OBJ) $(CLN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(LEX_OBJ) $(CLN_OBJ) $(LIBFT) -o $@

test_builtin: $(TEST_DIR)/test_builtin.c $(LIBFT) $(BLT_OBJ) $(ENV_OBJ) \
              $(CLN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(BLT_OBJ) $(ENV_OBJ) $(CLN_OBJ) \
		$(LIBFT) -o $@

test_parser: $(TEST_DIR)/test_parser.c $(LIBFT) $(PRS_OBJ) $(CLN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(PRS_OBJ) $(CLN_OBJ) $(LIBFT) -o $@

test_exec: $(TEST_DIR)/test_exec.c $(LIBFT) $(EXE_OBJ) $(BLT_OBJ) $(ENV_OBJ) \
           $(CLN_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(EXE_OBJ) $(BLT_OBJ) $(ENV_OBJ) \
		$(CLN_OBJ) $(LIBFT) -o $@

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(TESTS)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re tests