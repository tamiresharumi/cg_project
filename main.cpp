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
	(void)argc;
	(void)argv;

	std::vector<Objeto*> objetos;
    //Limits: a classe que faz as paredes da sala
    //e contem os limites da sala
	Limits limit(objetos);

	//rodando: variavel que mantem o programa aberto
	int rodando = 1;

	//angulo: angulo que o vetor visao da camera faz com o eixo x
	float angulo = 30.00f;

    //posicao e cor da luz LIGHT0
	float pLuz0[4] = {-2,(limit.getT())*0.5,2,1};
    float cLuz0[4] = {0.5,0.5,0.5,1};

    //posicao e cor da luz LIGHT1
	float pLuz1[4] = {0, limit.getT()/2, 0, 1};
    float cLuz1[4] = {0.6,0.6,0.4,1};

	float posObs[3] = {0, (limit.getT()/3), 0};

	float direcaoCamera[3] = {1,0,0};

	float raioColisao = 1.0;

	bool modoDebug = false;

	//inicializa a SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
	SDL_SetVideoMode(600, 600, 32, SDL_OPENGL);

	bool reflexao = false;
	{
		//checa se o stencil buffer foi criado direito pra poder habilitar a reflexão
		int stencilBits;
		SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &stencilBits);
		std::cout << "stencilBits: " << stencilBits << std::endl;
		if (stencilBits != 0)
		{
			reflexao = true;
			glClearStencil(0);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_COLOR_MATERIAL);

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

	//o chao é um objeto especial, usa pra fazer reflexão, então ele ganha uma variável só pra ele! ;)
	Objeto *objetoChao = new Objeto("p1.obj", Transformacao(0, 0, 0, 0), "floors/wood_floor.jpg");
	//objetos.push_back(objetoChao);    //chao OK
	objetos.push_back(new Objeto("p2.obj", Transformacao(0, 0, 0, 0)));                             //frente OK
	objetos.push_back(new Objeto("p3.obj", Transformacao(0, 0, 0, 0)));                             //direita OK
	objetos.push_back(new Objeto("p4.obj", Transformacao(0, 0, 0, 0)));                             //esquerda OK
	objetos.push_back(new Objeto("p5.obj", Transformacao(0, 0, 0, 0)));                             //tras OK
	objetos.push_back(new Objeto("p6.obj", Transformacao(0, 0, 0, 0)));                             //cima OK

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
				if (e.key.keysym.sym == SDLK_d && SDL_GetModState() & KMOD_CTRL)
					modoDebug = !modoDebug;
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

        //Define a cor que vai ficar a tela
		//glClearColor(1,0,0,0);
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

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

		if (reflexao)
		{
			//primeiro passo: marcar o stencil buffer
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			objetoChao->desenha();

			glDepthMask(GL_TRUE);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			//segundo passo: desenha a reflexão de todos os objetos na área
			//marcada do stencil buffer
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glPushMatrix();
				glScalef(1.0, -1.0, 1.0);
				glLightfv(GL_LIGHT0, GL_POSITION, pLuz0);
				glLightfv(GL_LIGHT1, GL_POSITION, pLuz1);
				for (unsigned i=0 ; i<objetos.size() ; ++i)
				{
					if (objetos[i] != objetoChao)
						objetos[i]->desenha();
				}
			glPopMatrix();
			glDisable(GL_STENCIL_TEST);
		}

        // Create light components
        GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat emissionLight[] = { 0.2f, 0.2f, 0.2f, 0.5f };
        // Assign created components to GL_LIGHT0
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
        glLightfv(GL_LIGHT0, GL_POSITION, pLuz0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, cLuz0);
		//glLightfv(GL_LIGHT0, GL_EMISSION, emissionLight);

        glLightfv(GL_LIGHT1, GL_POSITION, pLuz1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, cLuz1);

		if (reflexao)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			float corBlend[4] = {1,1,1,0.9};
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corBlend);
			glColor4f(1,1,1,0.9);
			objetoChao->desenha();
			corBlend[3] = 1;
			glColor4f(1,1,1,1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corBlend);
			glDisable(GL_BLEND);
		}

		//desenha todos os objetos
		for (unsigned i=0 ; i<objetos.size() ; ++i)
		{
			objetos[i]->desenha();
		}

		if (modoDebug)
		{
			for (unsigned i=0 ; i<objetos.size() ; ++i)
				objetos[i]->desenhaOBB();
			//desenha uma "esfera" na frente do personagem pra saber onde ele
			//vai bater..
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glColor3f(1,0,0);
			glPushMatrix();
			glTranslatef(posObs[0], posObs[1], posObs[2]);
			glRotatef(-angulo*180/M_PI, 0, 1, 0);
			glBegin(GL_LINE_STRIP);
			for (int i=0 ; i<=16 ; ++i)
				glVertex3f(raioColisao*cos(i * 2. * M_PI / 16.), 0, raioColisao*sin(i * 2. * M_PI / 16.));
			glEnd();
			glBegin(GL_LINE_STRIP);
			for (int i=0 ; i<=16 ; ++i)
				glVertex3f(0, raioColisao*cos(i * 2. * M_PI / 16.), raioColisao*sin(i * 2. * M_PI / 16.));
			glEnd();
			glBegin(GL_LINE_STRIP);
			for (int i=0 ; i<=16 ; ++i)
				glVertex3f(raioColisao*sin(i * 2. * M_PI / 16.), raioColisao*cos(i * 2. * M_PI / 16.), 0);
			glEnd();
			glColor3f(1,1,1);
			glPopMatrix();
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
		}

		//desenha nosso querido e super legal skybox!
		desenha_skybox(texturaSkybox, 50.0f);

//		desenha_grid();

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}
}

