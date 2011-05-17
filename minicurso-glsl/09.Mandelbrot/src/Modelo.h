#ifndef MODELO_H
#define MODELO_H

#include <cml/cml.h>

struct Vertice
{
	cml::vector3f posicao;
	cml::vector3f normal;
	cml::vector2f texCoord;
};

class Modelo
{
public:
	typedef std::vector<Vertice> Vertices;
	typedef std::vector<unsigned> Indices;

	void carrega(const Vertices &vertices, const Indices &indices);
	void desenha();
private:
	Vertices m_vertices;
	Indices m_indices;
};

#endif
