# super simple makefile
# call it using 'make NAME=name_of_code_file_without_extension'
# (assumes a .cpp extension)
NAME = game

SOURCE = main.cpp level.cpp window.cpp stickman.cpp game.cpp

all:
	@echo "Compiling..."
	g++ -o $(NAME) $(SOURCE) -L/usr/X11R6/lib -lX11 -lstdc++ 

run: all
	@echo "Running..."
	./$(NAME) 

