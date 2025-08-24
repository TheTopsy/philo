NAME = philo

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

SRC = philosophers.c philo_helper.c
OBJ = $(SRC:.c=.o)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
