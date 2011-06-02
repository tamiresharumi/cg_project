#include "objeto.h"
#include <GL/gl.h>
#include <SOIL/SOIL.h>
#include <limits>
#include <cmath>

Objeto::Objeto(const char *nomeModelo, Transformacao trans, const char *tex)
{
	modelo.carrega(nomeModelo);
	transformacao = trans;
	textura = 0;

	if (tex)
	{
		textura = SOIL_load_OGL_texture(tex, SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}
}

void Objeto::calculaAABB()
{
	//coloca os piores valores possíveis: com certeza vão ser sobrescritos no loop
	aabb.min[0] = aabb.min[1] = aabb.min[2] = std::numeric_limits<float>::max();
	aabb.max[0] = aabb.max[1] = aabb.max[2] = std::numeric_limits<float>::min();

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
	//agora que tem a AABB do objeto, tem que levar em conta a rotação dele pra
	//que na hora que for testar a colisão, testar com o objeto rotacionado, e
	//não o original
	
	//a rotação da transformação tá em graus, precisamos de radianos pra sin e cos
	float ang = transformacao.rotacao * M_PI / 180.0f;
	//a rotação é no eixo Y, então não precisa alterar o valor da coordenada Y. UHU!
	aabb.min[0] = +cos(ang) * aabb.min[0] + sin(ang) * aabb.min[2];
	aabb.min[2] = -sin(ang) * aabb.min[0] + cos(ang) * aabb.min[2];
	
	aabb.max[0] = +cos(ang) * aabb.max[0] + sin(ang) * aabb.max[2];
	aabb.max[2] = -sin(ang) * aabb.max[0] + cos(ang) * aabb.max[2];

	//por fim, leva em conta a translação do objeto
	for (int i=0 ; i<3 ; ++i)
		aabb.min[i] += transformacao.posicao[i];
	for (int i=0 ; i<3 ; ++i)
		aabb.max[i] += transformacao.posicao[i];
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

