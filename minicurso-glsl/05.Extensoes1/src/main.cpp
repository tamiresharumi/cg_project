#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>

PFNGLCREATESHADERPROC glCreateShader;

int main(int argc, char *argv[])
{
	const unsigned largura = 600;
	const unsigned altura = 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *tela = SDL_SetVideoMode(largura, altura, 32, SDL_OPENGL);

	glCreateShader = (PFNGLCREATESHADERPROC) SDL_GL_GetProcAddress("glCreateShader");
	if (!glCreateShader)
		return 10;
	unsigned shader = glCreateShader(GL_VERTEX_SHADER);

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

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}
