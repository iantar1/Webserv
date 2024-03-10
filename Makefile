NAME = webserv

CXX = c++

RM = rm -rf

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3

SRC =	./src/main.cpp \
  		./src/Server.cpp \
		./src/VirtualServer.cpp \
		./src/Response.cpp \
		./src/Location.cpp \
		./src/Client.cpp \
		./src/utils.cpp \
		./src/Request.cpp \
		#./src/PostResponse.cpp

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

COM = to save
git : 
	make fclean
	git add .
	git commit -m "${COM}"
	git push origin iantar1