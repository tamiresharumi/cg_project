#ifndef OBJETO_H
#define OBJETO_H

#include "modelo.h"
#include <string>

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

struct AABB
{
	float min[3];
	float max[3];
};

struct OBB
{
	//o "meio tamanho" da caixa
	float e[3];
	//posicao da AABB
	float centro[3];
	//eixos coordenados da AABB, três vetores de três componentes (x,y,z)
	float u[3][3];
};

class Objeto
{
public:
	Objeto(const char *nomeModelo, Transformacao trans, const char *tex=0);

	void calculaOBB();
	void desenhaOBB();
	void desenha();

	bool testaColisao(float posicao[3], float raio, float normal[3]);

	Transformacao transformacao;
	Modelo modelo;
	unsigned textura;
	OBB obb;
	std::string nome_modelo;
};

#endif

