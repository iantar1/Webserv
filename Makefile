NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

SRC =	main.cpp \
		src/Request.cpp \
		src/Response.cpp

OBJ = $(SRC:.cpp=.o)

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean $(NAME)

push :
	git add .
	git commit -m "Auto Commit"
	git push

.PHONY : all clean fclean re push
