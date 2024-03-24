NAME = webserv

CXX = c++

RM = rm -rf

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3

# SRC =	./src/VirtualServer.cpp \
# 		./src/Location.cpp \
#	   	./src/Server.cpp \

SRC = 	./src/Config.cpp\
		./src/main.cpp \
		./src/ServerBlock.cpp\
		./src/LocationBlock.cpp\
		./src/utilFunctions.cpp\
 		./src/Response.cpp \
 		./src/Delete.cpp \
 		./src/Request.cpp \
 		./src/utils.cpp \
 		./src/request_Utils.cpp \
 		./src/cgi_Response.cpp \
 		./src/Get.cpp \
 		./src/Post.cpp \
		./src/Client.cpp \


		
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
push : 
	make fclean
	git add .
	git commit -m "${COM}"
	git push origin iantar1