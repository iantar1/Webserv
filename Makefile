NAME = a.out

SRC = test.cpp

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ) 
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

