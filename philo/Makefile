NAME 		= philo
CC		= cc
CFLAGS 		= -Wall -Wextra -Werror -fsanitize=thread
SRCS		= philo.c utils.c init.c routines.c routines_utils.c
OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re