NAME 		= philo
CC		= cc
CFLAGS 		= -Wall -Wextra -Werror
SRCS		= philo.c utils.c \
		  
BONUSSRCS	= 


all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)

bonus: $(BONUSSRCS)
	$(CC) $(CFLAGS) $(BONUSSRCS) $(LIBFT) -o $(NAME)
clean:


fclean: clean
	rm -f $(NAME)

re: fclean all

rebonus: fclean bonus

.PHONY: all clean fclean bonus re rebonus