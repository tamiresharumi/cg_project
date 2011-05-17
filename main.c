#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "modelo.h"

int main(int argc, char *argv[])
{
	int rodando = 1;
	float angulo = 90.00f;
	float posPonto[3] = {0, 0, 0};
	float posLuz[4] = {0,0,2,0};
	float corLuz[4] = {1,1,1,1};
	float posObs[3] = {3, 0, 0};

	//inicializa a SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(400, 400, 32, SDL_OPENGL);

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	
	Modelo modelo;
	
	carrega_modelo(&modelo, "zen.obj");

	//loop pra manter o programa aberto
	while (rodando)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN){
				if (e.key.keysym.sym == SDLK_ESCAPE)
					rodando = 0;
				if(e.key.keysym.sym == SDLK_LEFT)
					posPonto[0]-=1.f;				
				if(e.key.keysym.sym == SDLK_RIGHT)
					posPonto[0]+=1.f;
				if(e.key.keysym.sym == SDLK_a){
					posObs[0]+=1.f;	
					printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
				}		
				if(e.key.keysym.sym == SDLK_s){
					posObs[1]-=1.f;
					printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
				}				
				if(e.key.keysym.sym == SDLK_d){
					posObs[0]-=1.f;	
					printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
				}
				if(e.key.keysym.sym == SDLK_w){
					posObs[1]+=1.f;	
					printf("posObs= {%f, %f, %f}\n", posObs[0], posObs[1], posObs[2]);
				}
			}			
			if (e.type == SDL_QUIT)
				rodando = 0;
		}
		glClearColor(0.3,0.4,0.3,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, 1, 0.5, 10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScalef(0.2,0.2,0.2);
		glTranslatef(2, 0, 0);
		gluLookAt(posObs[0], posObs[1], posObs[2],
				posPonto[0], posPonto[1], posPonto[2],
				0, 1, 0);
		glRotatef(angulo,0,1,0);

		glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);	
		desenha_modelo(&modelo);


		SDL_GL_SwapBuffers();
	}
}
