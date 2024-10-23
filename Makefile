NAME = ft_ssl

DBG		=	$(DBG_DIR)/$(NAME)

SRC_DIR =	src

BIN_DIR =	obj

DBG_DIR =	dbg

SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/ft_ssl.c \
	  $(SRC_DIR)/parsing.c \
	  $(SRC_DIR)/utils.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

DBG_OBJ = $(SRC:$(SRC_DIR)/%.c=$(DBG_DIR)/%.o)

DEPS = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.d)

DBG_DEPS = $(SRC:$(SRC_DIR)/%.c=$(DBG_DIR)/%.d)

INCLUDES = ./inc

CFLAGS = -Werror -Wextra -Wall -I$(INCLUDES)

all :	$(NAME)

debug	:	$(DBG)

$(BIN_DIR) $(DBG_DIR):
	@ mkdir -p $@

$(DBG_OBJ) : $(DBG_DIR)/%.o: $(SRC_DIR)/%.c | $(DBG_DIR)
	@ $(CC) $(CFLAGS) -g3 -MMD -c $< -o $@

$(OBJ) : $(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	@ $(CC) $(CFLAGS) -MMD -c $< -o $@

$(DBG) : $(DBG_OBJ)
	@ $(CC) $(CFLAGS) -g3 $(DBG_OBJ) -o $(DBG)
	@ echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mDone ! ✅"

$(NAME) : $(OBJ)
	@ $(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@ echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mDone ! ✅"

clean :
	@ rm -f $(OBJ)
	@ rm -f $(OBJ:.o=.d)
	@ rm -rf $(BIN_DIR) $(DBG_DIR)
	@echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mRemove binary files .. 🧹"

fclean : clean
	@ rm -f $(NAME)
	@echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mRemove executable .. 🗑️"

re : fclean all

-include	${DEPS} ${DBG_DEPS}

.PHONY: all clean fclean re debug