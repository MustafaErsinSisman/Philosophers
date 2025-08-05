NAME 		= deneme
CC			= cc
SRCS		= youtube.c
OBJS		= $(SRCS:.c=.o)
THREAD		= -pthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(THREAD) $(OBJS) -o $(NAME)

flag: CFLAGS = -Wall -Wextra -Werror -pthread
flag: fclean
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

run:
	@./${NAME}

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re flag run
