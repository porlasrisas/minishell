# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carbon <carbon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/07 15:57:01 by guigonza          #+#    #+#              #
#    Updated: 2025/07/15 21:20:55 by carbon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror 
LDFLAGS 	= -lreadline

SRC_DIR     = src
OBJ_DIR     = obj
LIBFT_DIR   = libft
INCLUDES    = -I. -Iincludes -I$(SRC_DIR) -I$(LIBFT_DIR)

# Archivos fuente listados explícitamente
SRC = $(SRC_DIR)/shell.c \
      $(SRC_DIR)/main.c \
      $(SRC_DIR)/parser.c \
	  $(SRC_DIR)/parse_tokens.c\
	  $(SRC_DIR)/cd.c \
	  $(SRC_DIR)/tests.c \
	  $(SRC_DIR)/parse_args.c \
	  $(SRC_DIR)/pipes.c \
      #$(SRC_DIR)/enviroment.c \
      $(SRC_DIR)/executor.c \
      $(SRC_DIR)/redirections.c \
	  $(SRC_DIR)/builtings.c \
      $(SRC_DIR)/signals.c

# Archivos objeto correspondientes
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

GREEN = \033[0;32m
YELLOW = \033[1;33m
RESET = \033[0m
RED = \033[0;31m

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME) $(INCLUDES) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)Compilación completada: $(NAME)$(RESET)"
	@echo "$(GREEN)---------------------------------------------------------------------------------------------------"
	@echo "$(YELLOW) ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓███████▓▒░░▒▓████████▓▒░░▒▓██████▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░    ░▒▓██▓▒░░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓▓█▓▒░░▒▓██▓▒░    ░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW) ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(GREEN)---------------------------------------------------------------------------------------------------"
	@echo "$(GREEN)\n ¡Enhorabuena champion, todo compilado!\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_DIR)/libft.a:
	@make -s -C $(LIBFT_DIR) bonus

clean:
	@make clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Objetos de minishell limpiados$(RESET)"

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)
	@echo "$(RED)Proyecto minishell completamente limpio$(RESET)"

re: fclean all

.PHONY: all clean fclean re