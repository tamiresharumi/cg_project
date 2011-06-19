#include "objeto.h"
#include <GL/gl.h>
#include <SOIL/SOIL.h>
#include <limits>
#include <cmath>
#include <iostream>

Objeto::Objeto(const char *nomeModelo, Transformacao trans, const char *tex)
{
	nome_modelo = nomeModelo;
	modelo.carrega(nomeModelo);
	transformacao = trans;
	textura = 0;

	if (tex)
	{
		textura = SOIL_load_OGL_texture(tex, SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}
}

//OBB: Object Bounding Box
void Objeto::calculaOBB()
{
	AABB aabb;

	//coloca os piores valores possíveis: com certeza vão ser sobrescritos no loop
	aabb.min[0] = aabb.min[1] = aabb.min[2] = +std::numeric_limits<float>::max();
	aabb.max[0] = aabb.max[1] = aabb.max[2] = -std::numeric_limits<float>::max();

	for (int i=0 ; i<modelo.numVerts ; ++i)
	{
		float *v = &modelo.verts[i*3];

		//busca o menor e o maior vértice aqui
		for (int j=0 ; j<3 ; ++j)
		{
			if (v[j] < aabb.min[j])
				aabb.min[j] = v[j];
			if (v[j] > aabb.max[j])
				aabb.max[j] = v[j];
		}
	}
#if TESTE
	std::cout << "AABB : ("
		<< aabb.min[0] << "," << aabb.min[1] << "," << aabb.min[2] << ") ("
		<< aabb.max[0] << "," << aabb.max[1] << "," << aabb.max[2] << ")\n";
#endif
	//agora que tem a AABB do objeto, tem que levar em conta a rotação dele pra
	//que na hora que for testar a colisão, testar com o objeto rotacionado, e
	//não o original

	for (int i=0 ; i<3 ; ++i)
		obb.e[i] = (aabb.max[i] - aabb.min[i])/2.0f;

	//a rotação da transformação tá em graus, precisamos de radianos pra sin e cos
	float ang = transformacao.rotacao * M_PI / 180.0f;
	obb.u[0][0] = -cos(ang);
	obb.u[0][1] = 0;
	obb.u[0][2] = sin(ang);
	obb.u[1][0] = 0;
	obb.u[1][1] = 1;
	obb.u[1][2] = 0;
	obb.u[2][0] = sin(ang);
	obb.u[2][1] = 0;
	obb.u[2][2] = cos(ang);

	//por fim, leva em conta a translação do objeto
	for (int i=0 ; i<3 ; ++i)
		obb.centro[i] = transformacao.posicao[i] + (aabb.min[i] + aabb.max[i]) / 2;
}

void Objeto::desenha()
{
	float *posicao = transformacao.posicao;
	float rotacao = transformacao.rotacao;

	if (!textura)
		glDisable(GL_TEXTURE_2D);
	else
		glBindTexture(GL_TEXTURE_2D, textura);

	//salva a matriz pra poder fazer as operações de matriz sem afetar
	//outros objetos
	glPushMatrix();
		glTranslatef(posicao[0], posicao[1], posicao[2]);
		glRotatef(rotacao, 0, 1, 0);
		modelo.desenha();
	glPopMatrix();

	if (!textura)
		glEnable(GL_TEXTURE_2D);
}

void Objeto::desenhaOBB()
{
	float vertices[][3] =
	{
		/*{aabb.min[0], aabb.min[1], aabb.min[2]},*/ {-obb.e[0], -obb.e[1], -obb.e[2]},
		/*{aabb.max[0], aabb.min[1], aabb.min[2]},*/ {+obb.e[0], -obb.e[1], -obb.e[2]},
		/*{aabb.min[0], aabb.max[1], aabb.min[2]},*/ {-obb.e[0], +obb.e[1], -obb.e[2]},
		/*{aabb.max[0], aabb.max[1], aabb.min[2]},*/ {+obb.e[0], +obb.e[1], -obb.e[2]},
		/*{aabb.min[0], aabb.min[1], aabb.max[2]},*/ {-obb.e[0], -obb.e[1], +obb.e[2]},
		/*{aabb.max[0], aabb.min[1], aabb.max[2]},*/ {+obb.e[0], -obb.e[1], +obb.e[2]},
		/*{aabb.min[0], aabb.max[1], aabb.max[2]},*/ {-obb.e[0], +obb.e[1], +obb.e[2]},
		/*{aabb.max[0], aabb.max[1], aabb.max[2]},*/ {+obb.e[0], +obb.e[1], +obb.e[2]},
	};

	short indices[][2] =
	{
		{0,1}, {0,2}, {0,4},
		{3,1}, {3,2}, {3,7},
		{6,2}, {6,4}, {6,7},
		{5,7}, {5,1}, {5,4}
	};

	glColor3f(1,0,0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(obb.centro[0], obb.centro[1], obb.centro[2]);
	glRotatef(transformacao.rotacao, 0, 1, 0);
	glBegin(GL_LINES);
		for (int i=0 ; i<12 ; ++i)
		{
			glVertex3fv(vertices[indices[i][0]]);
			glVertex3fv(vertices[indices[i][1]]);
		}
	glEnd();
	glPopMatrix();
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

bool Objeto::testaColisao(float posicao[3], float raio, float normal[3])
{
	//primeira parte: encontra o ponto na AABB mais próximo da posicao[3]
	float ponto[3];
	float d[3];

	for (int i=0 ; i<3 ; ++i)
	{
		d[i] = posicao[i] - obb.centro[i];
		ponto[i] = obb.centro[i];
	}

	for (int i=0 ; i<3 ; ++i)
	{
		#define PROD_ESCALAR(d,u) d[0]*u[0] + d[1]*u[1] + d[2]*u[2]
		float dist = PROD_ESCALAR(d, obb.u[i]);
		if (dist > obb.e[i]) dist = obb.e[i];
		if (dist < -obb.e[i]) dist = -obb.e[i];
		for (int j=0 ; j<3 ; ++j)
			ponto[j] += dist * obb.u[i][j];
		#undef PROD_ESCALAR
	}

	//calcula a distância (ao quadrado, pra economizar um sqrt) até a AABB
	float dist2 = 0;
	for (int i=0 ; i<3 ; ++i)
	{
		float d = ponto[i] - posicao[i];
		dist2 += d*d;
	}

	//terceira parte: checa se colidiu ou não. é só testar se a distância do
	//ponto até a AABB é menor que o raio da esfera
	bool colidiu = dist2 <= raio * raio;

	if (colidiu)
	{
		float comprimento = sqrt(dist2);
		float intrusao = raio - comprimento;
		//aqui é só pra calcular a normal da colisão, isto é, saber "de onde ela veio"
		//pra poder tratá-la no programa principal
		for (int i=0 ; i<3 ; ++i) {
			normal[i] = posicao[i] - ponto[i];
			normal[i] = normal[i] * intrusao / comprimento;
		}
	}

	return colidiu;
}

