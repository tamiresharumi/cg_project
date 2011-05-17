#include "Util.h"

#include <iostream>
#include <fstream>
#include <string>

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

void desenha_full_quad()
{
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

unsigned compilaShader(const char *arquivo, GLenum tipo)
{
	std::ifstream arq(arquivo);
	if (!arq)
	{
		printf("Arquivo %s nao existe\n", arquivo);
		return 0;
	}

	std::string texto(
		(std::istreambuf_iterator<char>(arq)),
		(std::istreambuf_iterator<char>())
	);

	const GLchar *source = texto.c_str();

	unsigned shader = glCreateShader(tipo);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	int tamanhoLog;
	char *log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tamanhoLog);
	if (tamanhoLog > 1)
	{
			log = new char[tamanhoLog];
			glGetShaderInfoLog(shader, tamanhoLog, 0, log);
			std::cerr << "InfoLog \"" << arquivo << "\"[" << tamanhoLog << "]:\n" << log << std::endl;
			delete [] log;
	}

	return shader;
}

unsigned criaPrograma(const char *arquivoVert, const char *arquivoFrag)
{
	unsigned vShader = compilaShader(arquivoVert, GL_VERTEX_SHADER);
	unsigned fShader = compilaShader(arquivoFrag, GL_FRAGMENT_SHADER);

	unsigned programa = glCreateProgram();

	glAttachShader(programa, vShader);
	glAttachShader(programa, fShader);
	glLinkProgram(programa);

	return programa;
}
