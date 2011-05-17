#include <SDL/SDL.h>
#include <GL/glew.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

unsigned compilaShader(const char *arquivo, GLenum tipo)
{
	std::ifstream arq(arquivo);
	if (!arq)
	{
		printf("Arquivo %s nao existe\n", arquivo);
		return 0;
	}

	std::string texto(
		(std::istreambuf_iterator<char>(arq)),
		(std::istreambuf_iterator<char>())
	);

	const GLchar *source = texto.c_str();

	unsigned shader = glCreateShader(tipo);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	return shader;
}

unsigned criaPrograma(const char *arquivoVert, const char *arquivoFrag)
{
	unsigned vShader = compilaShader(arquivoVert, GL_VERTEX_SHADER);
	unsigned fShader = compilaShader(arquivoFrag, GL_FRAGMENT_SHADER);

	unsigned programa = glCreateProgram();

	glAttachShader(programa, vShader);
	glAttachShader(programa, fShader);
	glLinkProgram(programa);

	return programa;
}

int main(int argc, char *argv[])
{
	const unsigned largura = 600;
	const unsigned altura = 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *tela = SDL_SetVideoMode(largura, altura, 32, SDL_OPENGL);

	GLenum glewErr = glewInit();
	if (glewErr != GLEW_NO_ERROR)
	{
		printf("Erro GLEW: %s\n", glewGetErrorString(glewErr));
		return 1;
	}

	if (!GLEW_VERSION_2_1)
	{
		printf("OpenGL 2.1 nao suportado! Fail!\n");
		return 1;
	}

	unsigned programa = criaPrograma("dados/vShader.vert", "dados/fShader.frag");

	float angulo = 0.0;

	bool rodando = true;
	while (rodando)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
				if (e.key.keysym.sym == SDLK_ESCAPE)
					rodando = false;
			if (e.type == SDL_QUIT)
				rodando = false;
		}

		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(angulo, 0, 0, 1);
		angulo += 0.5;

		glUseProgram(programa);
		glBegin(GL_TRIANGLES);
			glVertex2f(-0.5,-0.5);
			glVertex2f( 0.0,+0.5);
			glVertex2f(+0.5,-0.5);
		glEnd();

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}
