CC = cc

FLAGS = -Wall -Werror -Wextra 

NAME_1 = philo

all : $(NAME_1)

$(NAME_1) : 
	$(CC) $(FLAGS) philosophers.c philo_helper.c -o philo -g -pthread

clean :

fclean :
	rm -f $(NAME_1)

re : fclean all

.PHONY: all clean fclean re
