NAME = webserv

CXX = c++

RM = rm -rf

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3

# SRC =	./src/VirtualServer.cpp \
# 		./src/Location.cpp \

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
		./src/Client.cpp \
	   	./src/Server.cpp \
		./src/Desinged_Post.cpp \
		./src/index_cgi.cpp \
		# ./src/Post.cpp

%.o : %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

OBJ = ${SRC:.cpp=.o}

MSG = "Saving the Work"

${NAME} : ${OBJ}
	${CXX} ${CXXFLAGS} ${OBJ} -o ${NAME}

all : ${NAME}

clean :
	${RM} ${OBJ}

fclean : clean
	${RM} ${NAME}

re : fclean all

push : fclean
	git add .
	git commit -m ${MSG}
	git push
