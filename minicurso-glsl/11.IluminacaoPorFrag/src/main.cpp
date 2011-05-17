#include "Util.h"
#include "PlyLoader.h"

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

	glEnable(GL_DEPTH_TEST);

	unsigned programa = criaPrograma("dados/vShader.vert", "dados/fShader.frag");
	unsigned dirLuzLoc = glGetUniformLocation(programa, "dirLuz");

	Modelo *esfera = carregaPly("../dados/esfera-simples.ply");

	cml::vector3f dirLuz(0,0,1);

	cml::matrix44f_c projMat, cameraMat;
	cml::matrix_perspective_yfov_RH(projMat, cml::rad(30.0f),
		(float)largura/altura, 0.1f, 20.0f, cml::z_clip_neg_one);
	cml::matrix_look_at_RH(cameraMat,
		cml::vector3f(5,5,5), cml::vector3f(0,0,0), cml::vector3f(0,1,0));

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
		glLoadMatrixf(projMat.data());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(cameraMat.data());

		glUseProgram(programa);
		glUniform3fv(dirLuzLoc, 1, dirLuz.data());

		esfera->desenha();

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}
