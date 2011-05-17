#include "Util.h"
#include <GL/gl.h>

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
			glTexCoord2fv(texturas[j]);
			glVertex3fv(vertices[indices[i][j]]);
		}
	}
	glEnd();
}

void desenha_full_quad(unsigned tex)
{
	glBindTexture(GL_TEXTURE_2D, tex);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(-1,-1);
		glTexCoord2f(1,0); glVertex2f(+1,-1);
		glTexCoord2f(1,1); glVertex2f(+1,+1);
		glTexCoord2f(0,1); glVertex2f(-1,+1);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
