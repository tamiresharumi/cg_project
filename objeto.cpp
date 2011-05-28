#include "objeto.h"
#include <GL/gl.h>
#include <SOIL/SOIL.h>

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

