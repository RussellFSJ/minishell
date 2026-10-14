NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror

SRC_DIR     := src
BUILD_DIR   := build
INC_DIR     := include
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a

INCLUDES    := -I$(INC_DIR) -I$(LIBFT_DIR)/include
LDLIBS      := -lreadline

SRC_FILES   := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES   := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT) $(LDLIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
