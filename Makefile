NAME = webserv

CXX = c++

RM = rm -rf

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3

SRC = main.cpp ./src/parseRequest.cpp

%.o : %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

OBJ = ${SRC:.cpp=.o}

${NAME} : ${OBJ}
	${CXX} ${CXXFLAGS} ${OBJ} -o ${NAME}

all : ${NAME}

clean :
	${RM} ${OBJ}

fclean : clean
	${RM} ${NAME}

re : fclean all