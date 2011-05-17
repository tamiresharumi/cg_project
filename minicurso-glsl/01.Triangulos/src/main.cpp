#include <SDL/SDL.h>
#include <GL/gl.h>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *tela = SDL_SetVideoMode(600, 600, 32, SDL_OPENGL);
	float size = 0.5;

	bool rodando = true;
	while (rodando)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
				if (e.key.keysym.sym == SDLK_ESCAPE)
					rodando = false;
                else if (e.key.keysym.sym == SDLK_UP)
                    size = size + 0.1;
                else if (e.key.keysym.sym == SDLK_DOWN)
                    size = size - 0.1;
			if (e.type == SDL_QUIT)
				rodando = false;
		}

		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);

		//Agora algo legal! aqui nós só setamos as matrizes de projeção e
		//modelview pra identidade pra gente poder desenhar coisas bem simples
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Na prática, pra todo vértice que for desenhado agora, vai acontecer
		//assim:
		//	vertice_final = matriz_projection * matriz_modelview * vertice
		//como as duas matrizes são identidades, fica assim:
		//	vertice_final = vertice
		//aí a gente sabe que todos os vértices que vão aparecer na tela tem
		//que ser desenhados dentro do cubo x,y,z = [-1,1]
		//
		//Mais detalhes aqui:
		//	http://www.songho.ca/opengl/gl_transform.html
		//	http://www.opengl.org/resources/faq/technical/transformations.htm#tran0011

		//Desenha um triângulo só, bem no meio da tela
		glBegin(GL_TRIANGLES);
			//Isso faz o estado "cor" ser alterado pra (1,0,0). Todos os
			//vértices que forem desenhados depois disso serão vermelhos, até
			//que seja usado outro comando desse
			glColor3f(1,0,0);
			//A função glVertex2f coloca um vértice na cena, e o valor da
			//coordenada Z é usada como zero. Como estou desenhando triângulos,
			//o número de vértices total tem que ser múltiplo de 3
			glVertex2f(-0.5, -0.5);
			glVertex2f( 0.0, +0.5);
			glVertex2f(+0.5, -0.5);
		//Terminar de desenhar. Se não usar isso, nada aparece
		glEnd();

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}
