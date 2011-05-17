#include <SDL/SDL.h>
#include <GL/gl.h>
//Isso vai ser a nossa biblioteca de matrizes/vetores daqui pra frente
#include <cml/cml.h>

//Essa função desenha um cubo de lado 2*meioLado centrado na origem, então
//se alguém quiser desenhar o cubo em outra posição ou rotacionado, tem
//que usar as matrizes certas pra isso
void desenha_cubo(float meioLado)
{
	//Esses são os oito vértices do cubo
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

	//As seis normais do cubo
	float normais[][3] =
	{
		{-1,+0,+0},
		{+1,+0,+0},
		{+0,-1,+0},
		{+0,+1,+0},
		{+0,+0,-1},
		{+0,+0,+1},
	};

	//Cada linha representa uma face do cubo. Os quatro valores representam
	//quatro vértices, da lista de vértices dali de cima
	short indices[][4] =
	{
		{0, 2, 3, 1},
		{4, 6, 7, 5},
		{0, 1, 5, 4},
		{2, 3, 7, 6},
		{0, 4, 6, 2},
		{1, 5, 7, 3}
	};

	//Desenha o cubo, fazendo um acesso indexado aos vértices e normais
	glBegin(GL_QUADS);
	for (int i=0 ; i<6 ; ++i)
	{
		glNormal3fv(normais[i]);
		for (int j=0 ; j<4 ; ++j)
			glVertex3fv(vertices[indices[i][j]]);
	}
	glEnd();
}

int main(int argc, char *argv[])
{
	const unsigned largura = 600;
	const unsigned altura = 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *tela = SDL_SetVideoMode(largura, altura, 32, SDL_OPENGL);

	//Vamos usar essas matrizes como a matriz de projeção, a matriz modelview
	//da câmera e uma matriz de transformação do cubo
	cml::matrix44f_c matProj, matCamera, matCubo;

	//Isso é pra fazer o cubo girar
	float anguloCubo = 0.0f;

	//Agora que estamos desenhando objetos tridimensionais, haverá elementos que
	//aparecerão atrás de outros. Por conta disso, temos que habilitar o teste
	//de profundidade (zbuffer) pra que as coisas que estão atrás não sejam
	//desenhados na frente das coisas que estão mais perto da câmera
	glEnable(GL_DEPTH_TEST);
	//Pra deixar o demo mais interessante, já que estamos usando normais, vamos
	//também fazê-lo mais dinâmico. Pra isso usaremos luzes. Habilitamos aqui
	//a iluminação e a primeira luz
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//A posição e a cor da luz que será usada pra iluminar o ambiente. Note que
	//a última componente da "posLuz" é zero, indicando que essa luz não é uma
	//luz com uma posição, mas sim uma luz direcional, como um "sol"
	float posLuz[4] = {+1,+1,+1,0};
	float corLuz[4] = {1,1,1,1};

	//Esses comandos definem os parâmetros das luzes. O primeiro argumento
	//indica que vamos usar a primeira luz. Se não tivéssemos habilitado a
	//GL_LIGHT0 ali em cima, isso não teria nenhum efeito. O segundo argumento
	//indica o que deve ser alterado, e como o parâmetro para o tipo de luz se
	//chama GL_POSITION, a variável com os valores foi chamada posLuz, apesar
	//de se usar uma luz direcional
	glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);

	//A matriz de projeção será do tipo perspectiva
	cml::matrix_perspective_yfov_RH(matProj, cml::rad(60.0f),
		(float)largura/altura, 1.0f, 100.0f, cml::z_clip_neg_one);
	//A matriz da câmera é definida com essa função do tipo "look at", que
	//faz a câmera "olhar" para um ponto estando apoiada em outro
	cml::matrix_look_at_RH(matCamera,
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
		//Só habilitar o teste de profundidade não adianta. Pra que ele tenha
		//efeito, é necessário que o z buffer seja limpo toda vez também, para
		//que a posição dos objetos no frame anterior não "contamine" a cena
		//neste frame
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//essa é a matriz de transformação do cubo. Aqui, é apenas uma matriz
		//de rotação no eixo y
		cml::matrix_rotation_world_y(matCubo, anguloCubo);

		//Só aumenta o valor pra deixar a cena mais divertida
		anguloCubo += 0.01f;

		//As matrizes são carregadas agora a partir das matrizes criadas com
		//a cml, diretamente
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(matProj.data());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(matCamera.data());
		//Essa daqui não só é carregada, mas o resultado final é o seguinte:
		//	modelview = modelview * matCubo
		glMultMatrixf(matCubo.data());

		//Lembrando: a cor setada aqui não é alterada até que outro comando
		//glColor* seja usado, então o cubo será todo vermelho
		//O cubo desenhado tem lado 2, e está girando no eixo y por causa da
		//matriz modelview definida ali em cima
		desenha_cubo(1);

		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	SDL_Quit();

	return 0;
}
