OBJS = modelo.o main.o objeto.o \
	SOIL/SOIL.o SOIL/image_DXT.o SOIL/image_helper.o SOIL/stb_image_aug.o

SOIL/%.o: SOIL/%.c
	gcc -c $< -o $@

%.o: %.cpp
	g++ -I. -c $<

all: $(OBJS)
	g++ *.o SOIL/*.o -g3 -lSDL -lGLU -lGL
