#include <SDL/SDL.h>
#include <GL/gl.h>
#include <cml/cml.h>
//Isso é pra gente poder carregar texturas de um jeito fácil
#include <SOIL/SOIL.h>

void desenha_cubo(float meioLado)
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

	float normais[][3] =
	{
		{-1,+0,+0},
		{+1,+0,+0},
		{+0,-1,+0},
		{+0,+1,+0},
		{+0,+0,-1},
		{+0,+0,+1},
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

	//Agora são usadas também as coordenadas de textura para as faces
	//do cubo. Quatro linhas porque são quatro vértices por face
	float texturas[][2] =
	{
		{0.0,0.0},
		{1.0,0.0},
		{1.0,1.0},
		{0.0,1.0}
	};

	glBegin(GL_QUADS);
	for (int i=0 ; i<6 ; ++i)
	{
		glNormal3fv(normais[i]);
		for (int j=0 ; j<4 ; ++j)
		{
			//Agora seta a coordenada de textura antes do vértice
			glTexCoord2fv(texturas[j]);
			glVertex3fv(vertices[indices[i][j]]);
		}
	}
	glEnd();
}

//Essa função desenha um quadrado que ocupa a tela inteira. Isso é feito
//setando ambas a matriz de projeção e a modelview para a identidade e
//desenhando um quadrado com x,y=[-1,1]
void desenha_full_quad()
{
	//Faço glPushMatrix pra não estragar a matriz que tenha sido usada
	//do lado de fora dessa função
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Agora, além de desenhar os vértices, seta também as coordenadas uv
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(-1,-1);
		glTexCoord2f(1,0); glVertex2f(+1,-1);
		glTexCoord2f(1,1); glVertex2f(+1,+1);
		glTexCoord2f(0,1); glVertex2f(-1,+1);
	glEnd();
	
	//glPopMatrix pra deixar as matrizes de volta ao que eram
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

int main(int argc, char *argv[])
{
	const unsigned largura = 600;
	const unsigned altura = 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *tela = SDL_SetVideoMode(largura, altura, 32, SDL_OPENGL);

	cml::matrix44f_c matProj, matCamera, matCubo;

	float anguloCubo = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float posLuz[4] = {+1,+1,+1,0};
	float corLuz[4] = {1,1,1,1};

	glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);

	//Isso é pra habilitar o uso de texturas no programa
	glEnable(GL_TEXTURE_2D);
	//E isso serve pra carregar uma textura pro OpenGL. Tem uma série de
	//parâmetros pra essa função, mas esses já servem pra gente
	unsigned texId = SOIL_load_OGL_texture("../dados/teste.png",
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

		cml::matrix_perspective_yfov_RH(matProj, cml::rad(60.0f),
			(float)largura/altura, 1.0f, 100.0f, cml::z_clip_neg_one);
		cml::matrix_look_at_RH(matCamera,
			cml::vector3f(5,5,5), cml::vector3f(0,0,0), cml::vector3f(0,1,0));
		cml::matrix_rotation_world_y(matCubo, anguloCubo);

		anguloCubo += 0.01f;

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(matProj.data());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(matCamera.data());
		glMultMatrixf(matCubo.data());

		//Agora, ao invés de setar a cor do cubo, a gente aplica uma textura
		//nele. O comando glBindTexture serve pra dizer que essa textura deve
		//ser usada daqui pra frente, até que isso seja chamado de novo
		glBindTexture(GL_TEXTURE_2D, texId);
		desenha_cubo(1.0);

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}
