NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread
RM = rm -f

SRCDIR = .
OBJDIR = obj

SRCS = main.c utils.c init.c philo.c monitor.c

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re