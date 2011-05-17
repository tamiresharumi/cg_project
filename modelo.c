#include "modelo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>

#define COORDVERT 3
#define COORDNORM 3
#define COORDTEX 2
#define COORDFAC 12

int carrega_modelo(struct Modelo *modelo, const char *arquivo)
{
	FILE *arq = fopen(arquivo, "r");
	if (!arq)
	{
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
		printf("FUDEU! O arquivo obj não está correto!");
		return 0;
	}

	modelo->numFaces = nFaces;

	//agora aloca os buffers que precisa pra salvar tudo. É bem pesada essa
	//parte e provavelmente gera uma fragmentação de memória absurda, mas
	//vamos com isso por enquanto...
	modelo->verts = (float*) malloc(sizeof(float) * COORDVERT * nVerts);
	modelo->norms = (float*) malloc(sizeof(float) * COORDNORM * nNorms);
	modelo->tex = (float*) malloc(sizeof(float) * COORDTEX * nTex);
	modelo->faces = (int*) malloc(sizeof(int) * COORDFAC * nFaces);
	
	float *verts = modelo->verts;
	float *norms = modelo->norms;
	float *tex = modelo->tex;
	int *faces = modelo->faces;

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
					&tex[nTex*2], &tex[nTex*2+1]);
				nTex++;
			}
			else if (linha[0] == 'f' && linha[1] == ' ')
			{
				//a ordem dos parâmetros pra ler nesse sscanf é pra facilitar
				//a arrumação final no loop da seção de baixo
				lidos = sscanf(linha, "f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i",
					&faces[nFaces*12+0], &faces[nFaces*12+4], &faces[nFaces*12+8],
					&faces[nFaces*12+1], &faces[nFaces*12+5], &faces[nFaces*12+9],
					&faces[nFaces*12+2], &faces[nFaces*12+6], &faces[nFaces*12+10],
					&faces[nFaces*12+3], &faces[nFaces*12+7], &faces[nFaces*12+11]
					);
				nFaces++;
			}
		}
	}

	fclose(arq);

	//tudo ok
	return 1;
}

void apaga_modelo(struct Modelo *modelo)
{
	free(modelo->verts);
	free(modelo->norms);
	free(modelo->tex);
	free(modelo->faces);
}


void desenha_modelo(struct Modelo *modelo)
{
	int i, j;
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

