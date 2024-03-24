NAME = a.out

SRC = main.cpp ServerBlock.cpp LocationBlock.cpp Config.cpp utils.cpp

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 #-fsanitize=address

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ) 
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

