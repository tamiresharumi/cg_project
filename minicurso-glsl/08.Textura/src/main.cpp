#include "Util.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <cml/cml.h>
#include <cstdio>

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

	unsigned tex1Loc = glGetUniformLocation(programa, "textura1");
	unsigned tex2Loc = glGetUniformLocation(programa, "textura2");
	unsigned larguraLoc = glGetUniformLocation(programa, "larguraTela");

	unsigned textura1 = SOIL_load_OGL_texture("../dados/teste.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	unsigned textura2 = SOIL_load_OGL_texture("../dados/chao.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

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

		glUseProgram(programa);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textura2);
		glUniform1i(tex2Loc, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textura1);
		glUniform1i(tex1Loc, 0);

		glUniform1f(larguraLoc, largura);
		//full_quad não seta mais a textura
		desenha_full_quad();

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}
