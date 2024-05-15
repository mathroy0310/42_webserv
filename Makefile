# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/15 14:01:52 by maroy             #+#    #+#              #
#    Updated: 2024/05/15 18:26:32 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CCPP = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g -I ./include -I ./src #-fsanitize=address

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CCPP) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CCPP) $(CPPFLAGS) $(OBJS) -o $@
	@echo "$(GREEN)$(NAME) created !$(DEFAULT)"


clean:
	@rm -rf $(OBJ_DIR)
	@echo	"$(YELLOW)Object files deleted!$(DEFAULT)"

fclean: clean
	@rm -rf $(NAME)
	@echo	"$(RED)All deleted!$(DEFAULT)"

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./$(NAME)

leaks: all
	leaks --atExit -- ./$(NAME)

re: fclean all

.PHONY: all clean fclean leaks re

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
DARKGRAY= \033[1;30m
DEFAULT = \033[1;30m
