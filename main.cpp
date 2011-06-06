#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "modelo.h"
#include <vector>
#include <cmath>
#include <SOIL/SOIL.h>
#include "limits.h"
#include <cstdio>
#include <iostream>
#include "objeto.h"

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

void desenha_skybox(unsigned skyboxTex, float meioLado)
{
	float vertices[][3] =
	{
		{-meioLado, -meioLado, -meioLado}, //0 TRAS BAIXO ESQ
		{-meioLado, -meioLado, +meioLado}, //1 FRENTE BAIXO ESQ
		{-meioLado, +meioLado, -meioLado}, //2 TRAS CIMA ESQ
		{-meioLado, +meioLado, +meioLado}, //3 FRENTE CIMA ESQ
		{+meioLado, -meioLado, -meioLado}, //4 TRAS BAIXO DIR
		{+meioLado, -meioLado, +meioLado}, //5 FRENTE BAIXO DIR
		{+meioLado, +meioLado, -meioLado}, //6 TRAS CIMA DIR
		{+meioLado, +meioLado, +meioLado}  //7 FRENTE CIMA DIR
	};

	float texturas[][3] =
	{
		{-1, -1, -1},
		{-1, -1, +1},
		{-1, +1, -1},
		{-1, +1, +1},
		{+1, -1, -1},
		{+1, -1, +1},
		{+1, +1, -1},
		{+1, +1, +1},
	};

	short indices[][4] =
	{
		{0, 2, 3, 1},
		{4, 6, 7, 5},
		{0, 1, 5, 4},
		{2, 3, 7, 6},
		{0, 4, 6, 2},
		{1, 5, 7, 3}
	};

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
	glBegin(GL_QUADS);
	for (int i=0 ; i<6 ; ++i)
	{
		for (int j=0 ; j<4 ; ++j)
		{
			//Agora seta a coordenada de textura antes do vértice
			glTexCoord3fv(texturas[indices[i][j]]);
			glVertex3fv(vertices[indices[i][j]]);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_LIGHTING);
}



int main(int argc, char *argv[])
{
	Limits limit;
	int rodando = 1;
	float angulo = 30.00f;
	float posPonto[3] = {0, 0, 0};
	float posLuz[4] = {-2,limit.getT(),2,0};
	float posLuz1[4] = {0, limit.getT()/2, 0, 1};
	float corLuz[4] = {1,1,1,1};
	float corLuz1[4] = {0.9,0.9,0.6,0};

	float posObs[3] = {0, (limit.getT()/3), -3};

	float direcaoCamera[3] = {1,0,0};

	float raioColisao = 1.0;

	//inicializa a SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(600, 600, 32, SDL_OPENGL);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//textura do skybox, criada a partir das seis imagens
	unsigned texturaSkybox = SOIL_load_OGL_cubemap(
		"skybox/esquerda.jpg",
		"skybox/direita.jpg",
		"skybox/cima.jpg",
		"skybox/baixo.jpg",
		"skybox/tras.jpg",
		"skybox/frente.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);

	std::vector<Objeto*> objetos;
	objetos.push_back(new Objeto("models/t_sofa3.obj", Transformacao(30, 0, 0, 0), "leathers/glass_leather.jpg"));
	objetos.push_back(new Objeto("models/t_table.obj", Transformacao(0, 5, 0, 0)));
	//objetos.push_back(new Objeto("models/floor_lamp.obj", Transformacao(0, -5, 0, 0)));
//	objetos.push_back(new Objeto("p1.obj", Transformacao(0, 0, 0, 0), "floors/wood_floor.jpg"));    //chao OK
//	objetos.push_back(new Objeto("p2.obj", Transformacao(0, 0, 0, 0)));                             //frente OK
//	objetos.push_back(new Objeto("p3.obj", Transformacao(0, 0, 0, 0)));                             //direita OK
//	objetos.push_back(new Objeto("p4.obj", Transformacao(0, 0, 0, 0)));                             //esquerda OK
//	objetos.push_back(new Objeto("p5.obj", Transformacao(0, 0, 0, 0)));                             //tras OK
//	objetos.push_back(new Objeto("p6.obj", Transformacao(0, 0, 0, 0)));                             //cima OK

	//calcula a AABB de todo mundo pra poder testar contra colisões depois
	for (unsigned i=0 ; i<objetos.size() ; ++i)
		objetos[i]->calculaOBB();

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
			angulo -= .03;
		if (teclado[SDLK_RIGHT])
			angulo += .03;

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

		//checa se bateu em alguma coisa
		for (unsigned i=0 ; i<objetos.size() ; ++i)
		{
			float normal[3];
			if (objetos[i]->testaColisao(posObs, raioColisao, normal))
			{
				posObs[0] += normal[0];
				posObs[2] += normal[2];
			}
		}

		glClearColor(1,0,0,0);
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//Na projection, sempre coloca só a gluPerspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//faz a câmera ficar com perspectiva
		gluPerspective(60, 1, 0.5, 100*1.4142);

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

//		gluLookAt(7, 2, 7, 0, 0, 0, 0, 1, 0);

		glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
        glDisable(GL_LIGHT0);

        glLightfv(GL_LIGHT1, GL_POSITION, posLuz1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, corLuz1);

		//desenha todos os objetos
		for (unsigned i=0 ; i<objetos.size() ; ++i)
		{
			objetos[i]->desenha();
			objetos[i]->desenhaOBB();
		}
		
		//desenha uma "esfera" na frente do personagem pra saber onde ele
		//vai bater..
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,0,0);
		glBegin(GL_LINE_STRIP);
		for (int i=0 ; i<=16 ; ++i)
			glVertex3f(posObs[0] + raioColisao*cos(i * 2. * M_PI / 16.), posObs[1], posObs[2] + raioColisao*sin(i * 2. * M_PI / 16.));
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (int i=0 ; i<=16 ; ++i)
			glVertex3f(posObs[0], posObs[1] + raioColisao*cos(i * 2. * M_PI / 16.), posObs[2] + raioColisao*sin(i * 2. * M_PI / 16.));
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (int i=0 ; i<=16 ; ++i)
			glVertex3f(posObs[0] + raioColisao*sin(i * 2. * M_PI / 16.), posObs[1] + raioColisao*cos(i * 2. * M_PI / 16.), posObs[2]);
		glEnd();
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		//desenha nosso querido e super legal skybox!
		desenha_skybox(texturaSkybox, 50.0f);

//		desenha_grid();

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}
}

