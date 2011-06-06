EXE = a.out

OBJS = modelo.o main.o objeto.o limits.o \
	SOIL/SOIL.o SOIL/image_DXT.o SOIL/image_helper.o SOIL/stb_image_aug.o

SOIL/%.o: SOIL/%.c
	gcc -c $< -g3 -o $@

%.o: %.cpp
	g++ -I. -c -g3 -Wall -Wextra $<

all: $(EXE)

$(EXE): $(OBJS)
	g++ *.o SOIL/*.o -g3 -lSDL -lGLU -lGL
