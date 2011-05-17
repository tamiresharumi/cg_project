#include "Modelo.h"
#include <cassert>
#include <GL/gl.h>

void Modelo::carrega(const Vertices &vertices, const Indices &indices)
{
	m_vertices = vertices;
	m_indices = indices;
}

void Modelo::desenha()
{
	unsigned numVerts = m_indices.size() / 3;

	//Tem que ser múltiplo de três pra formar triângulos
	assert(m_indices.size() % 3 == 0);

	glBegin(GL_TRIANGLES);
	for (unsigned i=0 ; i<numVerts ; ++i)
	{
		for (unsigned j=0 ; j<3 ; ++j)
		{
			Vertice &v = m_vertices[m_indices[i*3 + j]];
			glTexCoord2fv(v.texCoord.data());
			glNormal3fv(v.normal.data());
			glVertex3fv(v.posicao.data());
		}
	}
	glEnd();
}
