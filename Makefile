# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/15 14:01:52 by maroy             #+#    #+#              #
#    Updated: 2024/03/26 01:14:38 by rmarceau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CCPP = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++11 -g -fsanitize=address  -I ./include

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

re: fclean all

.PHONY: all clean fclean re

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
DARKGRAY= \033[1;30m
DEFAULT = \033[1;30m
