#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "modelo.h"
#include <vector>
#include <cmath>
#include <SOIL/SOIL.h>

struct Transformacao
{
	Transformacao()
	{ }
	Transformacao(float rot, float x, float y, float z)
	{
		rotacao = rot;
		posicao[0] = x;
		posicao[1] = y;
		posicao[2] = z;
	}
	//angulo e direcao
	float rotacao;
	float posicao[3];
};

class Objeto
{
public:
	Objeto(const char *nomeModelo, Transformacao trans)
	{
		modelo.carrega(nomeModelo);
		transformacao = trans;
	}

	void desenha()
	{
		glMatrixMode(GL_MODELVIEW);
	}

	Transformacao transformacao;
	Modelo modelo;
};

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
	int rodando = 1;
	float angulo = 90.00f;
	float posPonto[3] = {0, 0, 0};
	float posLuz[4] = {0,0,2,0};
	float corLuz[4] = {1,1,1,1};

	float posObs[3] = {5, 3, 0};

	float direcaoCamera[3] = {1,0,0};

	//inicializa a SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(600, 600, 32, SDL_OPENGL);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	std::vector<Objeto*> objetos;
	objetos.push_back(new Objeto("t_sofa3.obj", Transformacao(0, 0, 0, 0)));
	objetos.push_back(new Objeto("t_table.obj", Transformacao(0, 5, 0, 0)));

	unsigned texId = SOIL_load_OGL_texture("leather.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
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

		glBindTexture(GL_TEXTURE_2D, texId);

		//desenha todos os objetos
		for (int i=0 ; i<objetos.size() ; ++i)
		{
			float *posicao = objetos[i]->transformacao.posicao;
			float rotacao = objetos[i]->transformacao.rotacao;

			//salva a matriz pra poder fazer as operações de matriz sem afetar
			//outros objetos
			glPushMatrix();
				glTranslatef(posicao[0], posicao[1], posicao[2]);
				glRotatef(rotacao, 0, 1, 0);
				objetos[i]->modelo.desenha();
			glPopMatrix();
		}

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
