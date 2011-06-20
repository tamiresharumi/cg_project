#include "modelo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>

/*vertices = v;
  normais = vn;
  texturas = vt;
  faces = f;
  */
#define COORDVERT 3
#define COORDNORM 3
#define COORDTEX 2
#define COORDFAC 9


int Modelo::carrega(const char *arquivo)
{
	FILE *arq = fopen(arquivo, "r");
	if (!arq)
	{
	    printf("Nao abre o arquivo %s\n", arquivo);
	    perror("Falha em " __FILE__ "");
		return 0;
	}

	//primeiro conta quantos tem de cada elemento
	int nVerts=0, nNorms=0, nTex=0, nFaces=0;
	while (!feof(arq))
	{
		char linha[255];
		if (fgets(linha, 255, arq))
		{
			if (linha[0] == 'v' && linha[1] == ' ')
				nVerts++;
			else if (linha[0] == 'v' && linha[1] == 'n')
				nNorms++;
			else if (linha[0] == 'v' && linha[1] == 't')
				nTex++;
			else if (linha[0] == 'f' && linha[1] == ' ')
				nFaces++;
		}
	}
	//se tiver 0 de algum deles, o arquivo não tá do jeito que eu quero...
	if (!nVerts || !nNorms || !nTex || !nFaces)
	{
		fclose(arq);
		printf("Falta algum parametro em %s, ve isso ai\n", arquivo);
		return 0;
	}

	this->numFaces = nFaces;
	this->numVerts = nVerts;

	//agora aloca os buffers que precisa pra salvar tudo. É bem pesada essa
	//parte e provavelmente gera uma fragmentação de memória absurda, mas
	//vamos com isso por enquanto...
	verts = new float[COORDVERT * nVerts];
	norms = new float[COORDNORM * nNorms];
	tex = new float[COORDTEX * nTex];
	faces = new int[COORDFAC * nFaces];

	//Lê todo o arquivo de novo e vai preenchendo os buffers dessa vez
	rewind(arq);
	nVerts = nNorms = nTex = nFaces = 0;
	while (!feof(arq))
	{
		char linha[255];
		int lidos;
		if (fgets(linha, 255, arq))
		{
			if (linha[0] == 'v' && linha[1] == ' ')
			{
				lidos = sscanf(linha, "v %f %f %f",
					&verts[nVerts*COORDVERT], &verts[nVerts*COORDVERT+1], &verts[nVerts*COORDVERT+2]);
				nVerts++;
			}
			else if (linha[0] == 'v' && linha[1] == 'n')
			{
				lidos = sscanf(linha, "vn %f %f %f",
					&norms[nNorms*COORDNORM], &norms[nNorms*COORDNORM+1], &norms[nNorms*COORDNORM+2]);
				nNorms++;
			}
			else if (linha[0] == 'v' && linha[1] == 't')
			{
				lidos = sscanf(linha, "vt %f %f",
					&tex[nTex*COORDTEX], &tex[nTex*COORDTEX+1]);
				nTex++;
			}
			else if (linha[0] == 'f' && linha[1] == ' ')
			{
				//a ordem dos parâmetros pra ler nesse sscanf é pra facilitar
				//a arrumação final no loop da seção de baixo
				lidos = sscanf(linha, "f %i/%i/%i %i/%i/%i %i/%i/%i",
					&faces[nFaces*COORDFAC+0], &faces[nFaces*COORDFAC+3], &faces[nFaces*COORDFAC+6],
					&faces[nFaces*COORDFAC+1], &faces[nFaces*COORDFAC+4], &faces[nFaces*COORDFAC+7],
					&faces[nFaces*COORDFAC+2], &faces[nFaces*COORDFAC+5], &faces[nFaces*COORDFAC+8]
					);
				nFaces++;
			}
		}
	}

	fclose(arq);

	//tudo ok
	return 1;
}

Modelo::~Modelo()
{
	delete []this->verts;
	delete []this->norms;
	delete []this->tex;
	delete []this->faces;
}


void Modelo::desenha(void)
{
	int i, j;
	Modelo *modelo = this;
	glBegin(GL_TRIANGLES);

	for (i=0 ; i<modelo->numFaces ; ++i)
	{
		for (j=0 ; j<3 ; ++j)
		{
			glNormal3fv(
				&modelo->norms[
					3*(modelo->faces[i*COORDFAC + j + 6] - 1)
				]
			);
			glTexCoord2fv(
				&modelo->tex[
					2*(modelo->faces[i*COORDFAC + j + 3] - 1)
				]
			);
			glVertex3fv(
				&modelo->verts[
					3*(modelo->faces[i*COORDFAC + j] - 1)
				]
			);
		}
	}

	glEnd();
}


/*
void Modelo::desenha()
{
	int i, j;
	Modelo *modelo = this;
	glBegin(GL_QUADS);

	for (i=0 ; i<modelo->numFaces ; ++i)
	{
		for (j=0 ; j<4 ; ++j)
		{
			glNormal3fv(
				&modelo->norms[
					3*(modelo->faces[i*12 + j + 8] - 1)
				]
			);
			glTexCoord2fv(
				&modelo->tex[
					2*(modelo->faces[i*12 + j + 4] - 1)
				]
			);
			glVertex3fv(
				&modelo->verts[
					3*(modelo->faces[i*12 + j] - 1)
				]
			);
		}
	}

	glEnd();
}
*/
