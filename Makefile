CC=		g++
FLAGS=	-Wall -Wextra -std=c++11 -O3 -g
NAME=	ps

SRC= main.cpp \
	 OpenCLError.cpp \
	 OpenCLSimulation.cpp \
	 OpenCLTask.cpp \
	 OpenCLTaskPInit.cpp \
	 OpenCLTaskPMove.cpp \
	 Matrix.cpp \
	 OpenGLManager.cpp \
	 OpenGLMatrix.cpp \
	 OpenGLScene.cpp \
	 OpenGLShader.cpp \
	 vector.cpp \
	 Control.cpp \
	 CameraFree.cpp \
	 Timer.cpp

OBJ= $(SRC:.cpp=.o)

INC=	-I includes -I includes/GLFW
LIB=	-L libs/ -lglad -lglfw3

FRWK=	-framework OpenCL \
		-framework AGL \
		-framework Cocoa \
		-framework OpenGL \
		-framework IOKit \
		-framework CoreFoundation \
		-framework CoreVideo

.PHONY: clean fclean re
.SILENT:

all:$(NAME) 

$(NAME): $(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LIB) $(FRWK)
		echo "\t\xF0\x9F\x8F\x81   Compiling \033[35m$(NAME) \033[0mDONE!"

%.o: %.cpp
		echo "\t\xF0\x9F\x94\xA7   Building \033[34m $@ \033[0m" 
		$(CC) -c $(FLAGS) $(INC) -o $@ $<

clean:
		echo "\t\xF0\x9F\x92\xA3   Cleaning"
		rm -rf $(OBJ)

fclean: clean
		echo "\t\xF0\x9F\x9A\xBD   Full Clean"
		rm -rf $(NAME)

re: fclean all
