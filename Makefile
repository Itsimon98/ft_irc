NAME = ircserv
CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g
SRCS = main.cpp server.cpp utils.cpp user.cpp channel.cpp parser.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
