#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "modelo.h"

#if 0
struct Transformacao
{
	//angulo e direcao
	float rotacao[4];
	float posicao[3];
};

class Objeto
{
public:
	Objeto(const char *nomeModelo)
	{
		modelo.carrega(nomeModelo);
	}

	void desenha()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(posicao[0], posicao[1], posicao[2]);
		glRotatef(rotacao[0], rotacao[1], rotacao[2], rotacao[4]);
		glPopMatrix();
	}

	Transformacao transformacao;
	Modelo modelo;
};
#endif

void desenha_grid()
{
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	glBegin(GL_LINES);
		for (int i=-10 ; i<10 ; ++i)
		{
			glVertex3f(i,0,-10);
			glVertex3f(i,0,10);

			glVertex3f(-10,0,i);
			glVertex3f(10,0,i);
		}
	glEnd();
	glEnable(GL_LIGHTING);
}

int main(int argc, char *argv[])
{
	int rodando = 1;
	float angulo = 90.00f;
	float posPonto[3] = {0, 0, 0};
	float posLuz[4] = {0,0,2,0};
	float corLuz[4] = {1,1,1,1};

	float posObs[3] = {5, 3, 0};

	//inicializa a SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(600, 600, 32, SDL_OPENGL);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	Modelo modelo;
	Modelo modelo2;

	modelo.carrega("t_sofa3.obj");
    modelo2.carrega("t_table.obj");

	//loop pra manter o programa aberto
	while (rodando)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN){
				if (e.key.keysym.sym == SDLK_ESCAPE)
					rodando = 0;
			}
			if (e.type == SDL_QUIT)
				rodando = 0;
		}

		Uint8 *teclado = SDL_GetKeyState(0);
		
		if (teclado[SDLK_LEFT])
			angulo -= 1;
		if (teclado[SDLK_RIGHT])
			angulo += 1;
		if (teclado[SDLK_UP])
			posObs[2] += 1;
		if (teclado[SDLK_DOWN])
			posObs[2] -= 1;
		if (teclado[SDLK_a]) {
			posObs[0]+=1.f;
			//printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
		}
		if(teclado[SDLK_s]) {
			posObs[1]-=1.f;
			//printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
		}
		if(teclado[SDLK_d]) {
			posObs[0]-=1.f;
			//printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
		}
		if(teclado[SDLK_w]) {
			posObs[1]+=1.f;
			//printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
		}

		glClearColor(0.3,0.4,0.3,0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Na projection, sempre coloca só a gluPerspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//faz a câmera ficar com perspectiva
		gluPerspective(60, 1, 0.5, 100);

		//Na modelview, as operações com matrizes tem que ser nessa ordem:
		//	1. carregar identidade
		//	2. usar a gluLookAt pra colocar a câmera no lugar certo
		//	3. fazer as transformações do objeto
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(posObs[0], posObs[1], posObs[2],
                0, 0, 0,
				0, 1, 0);

		//faz as coisas girarem
		glRotatef(angulo,0,1,0);

		glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
		modelo.desenha();
		//modelo2.desenha();
/*
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScalef(0.8,0.8,0.8);
		glTranslatef(-1, 0, 0);

		gluLookAt(posObs[0], posObs[1], posObs[2],
                0, 0, 0,
				0, 1, 0);

		//faz as coisas girarem
		glRotatef(angulo,0,1,0);

		glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
		modelo2.desenha();
*/
		desenha_grid();
#if 0
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScalef(0.8,0.8,0.8);
		glTranslatef(-1, 0, 0);
		gluLookAt(posObs[0], posObs[1], posObs[2],
                0, 0, 0,
				0, 1, 0);

		glBegin(GL_QUADS);
			glNormal3f(0,0,-1);
			glVertex3f(5,  0, 5);
			glVertex3f(5, 10, 5);
			glVertex3f(6, 10, 5);
			glVertex3f(6,  0, 5);
		glEnd();
#endif

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}
}
