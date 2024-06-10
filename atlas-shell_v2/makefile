CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
NAME = hsh
RM = rm -f

.PHONY: all clean oclean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)


clean:
	$(RM) $(OBJ)

oclean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all