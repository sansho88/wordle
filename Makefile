NAME:= wordle
FLAGS:= -Wall -Wextra -Werror -std=c++11

SRCS= Letter.cpp main.cpp
HEADERS= Letter.hpp
OBJS= $(SRCS:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	c++ $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	c++ $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re