#ifndef OBJETO_H
#define OBJETO_H

#include "modelo.h"

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
	Objeto(const char *nomeModelo, Transformacao trans, const char *tex=0);

	void desenha();

	Transformacao transformacao;
	Modelo modelo;
	unsigned textura;
};

#endif

