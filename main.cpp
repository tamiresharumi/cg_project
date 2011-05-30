#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "modelo.h"
#include <vector>
#include <cmath>
#include <SOIL/SOIL.h>
#include "objeto.h"
#include "limits.h"
void desenha_grid()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
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
	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
	Limits limit;
	int rodando = 1;
	float angulo = 90.00f;
	float posPonto[3] = {0, 0, 0};
	float posLuz[4] = {-2,12,2,0};
	float corLuz[4] = {1,1,1,1};
	float corLuz1[4] = {0.9,0.9,0.9,0};

	float posObs[3] = {5, 3, 0};

	float direcaoCamera[3] = {1,0,0};

	//inicializa a SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(600, 600, 32, SDL_OPENGL);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	std::vector<Objeto*> objetos;
//	objetos.push_back(new Objeto("models/t_sofa3.obj", Transformacao(0, 0, 0, 0), "leathers/glass_leather.jpg"));
//	objetos.push_back(new Objeto("models/t_table.obj", Transformacao(0, 5, 0, 0)));
//	objetos.push_back(new Objeto("models/floor_lamp.obj", Transformacao(0, -5, 0, 0)));
	objetos.push_back(new Objeto("p0.obj", Transformacao(0, 0, 0, 0)));
	objetos.push_back(new Objeto("p6.obj", Transformacao(0, 0, 0, 0)));

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
			angulo -= .01;
		if (teclado[SDLK_RIGHT])
			angulo += .01;

		if (teclado[SDLK_a])
		{
			posObs[0] += 0.1*direcaoCamera[2];
			posObs[2] -= 0.1*direcaoCamera[0];
		}
		if (teclado[SDLK_s])
		{
			posObs[0] -= 0.1*direcaoCamera[0];
			posObs[2] -= 0.1*direcaoCamera[2];
		}
		if (teclado[SDLK_d])
		{
			posObs[0] -= 0.1*direcaoCamera[2];
			posObs[2] += 0.1*direcaoCamera[0];
		}
		if (teclado[SDLK_w])
		{
			posObs[0] += 0.1*direcaoCamera[0];
			posObs[2] += 0.1*direcaoCamera[2];
		}

		//atualiza a direção da camera
		direcaoCamera[0] = cos(angulo);
		direcaoCamera[1] = 0;
		direcaoCamera[2] = sin(angulo);

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
		//
		//Tem uma função que chama glPushMatrix, que serve pra copiar a matriz
		//modelview pra poder restaurá-la depois. Dá pra salvar, fazer um monte
		//de operações nela, e depois que for usada essa cópia, pode restaurar a
		//versão anterior. Isso deixa a gente fazer o seguinte:
		//
		//	//define a posição da camera
		//	glLoadIdentity();
		//	gluLookAt(...);
		//
		//	//define a posicao do objeto 1
		//	glPushMatrix();
		//		glTranslate(...); //(1)
		//		glRotate(...);  //(2)
		//		objeto1.desenha();
		//	glPopMatrix();
		//
		//	//define a posicao do objeto 2. O legal aqui é que, por causa do
		//	//gl{Push/Pop}Matrix, as operações marcadas com (1) e (2) não afetam
		//	//o desenhar desse segundo objeto!
		//	glPushMatrix();
		//		glTranslate(...);
		//		glRotate(...);
		//		objeto2.desenha();
		//	glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		float focoCamera[3] = {
			posObs[0] + direcaoCamera[0],
			posObs[1] + direcaoCamera[1],
			posObs[2] + direcaoCamera[2]
		};
		gluLookAt(
			posObs[0], posObs[1], posObs[2],
			focoCamera[0], focoCamera[1], focoCamera[2],
			0, 1, 0);

		glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
        glDisable(GL_LIGHT0);

        glLightfv(GL_LIGHT1, GL_POSITION, posLuz);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, corLuz1);

		//desenha todos os objetos
		for (int i=0 ; i<objetos.size() ; ++i)
			objetos[i]->desenha();

		desenha_grid();
#if 0
		//Isso seria pra desenhar uma parede, por exemplo.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
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

