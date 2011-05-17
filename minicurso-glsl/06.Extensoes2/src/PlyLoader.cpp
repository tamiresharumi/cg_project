#include "PlyLoader.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

bool verificaPropriedade(const char *linha, const char *prop)
{
	char buf[0xF];
	int res = sscanf(linha, "property float %s", buf);

	if (res == 1 && strcmp(prop, buf) == 0)
		return true;
	else
		return false;
}

bool starts_with(const char *input, const char *test)
{
	const char *s = test;
	const char *i = input;

	const char *sfim = test + strlen(test);
	const char *ifim = input + strlen(input);

	while (s != sfim && i != ifim)
	{
		if (*s++ != *i++)
			return false;
	}

	return s == sfim;
}

Modelo* carregaPly(const char *arquivo)
{
	char buffer[0xFF];
	char texto[0xFF];
	int numVertices = 0;
	int numFaces = 0;

	std::string arq(arquivo);
	FILE *in = fopen(arquivo, "r");
	if (!in)
	{
		throw std::runtime_error("Nao foi possivel abrir \"" + arq + "\"");
	}

	//checa se é um ply mesmo
	fgets(buffer, 0xFF, in);
	sscanf(buffer, "%s", texto);
	if (0 != strcmp(texto, "ply"))
		throw std::runtime_error("O arquivo \"" + arq + "\" nao e' um ply");
	//checa se é um ascii 1.0
	int maior, menor;
	fgets(buffer, 0xFF, in);
	if (sscanf(buffer, " format ascii %i.%i", &maior, &menor) != 2)
	{
		throw std::runtime_error("Nao e' um ply ascii");
	}

	if (maior != 1 || menor != 0)
	{
		throw std::runtime_error("Nao e' um ply ascii 1.0");
	}

	//começa a ler o arquivo pra valer, uhu!
	std::vector<std::string> linhas;

	while (fgets(buffer, 0xFF, in) == buffer)
	{
		if (starts_with(buffer, "comment"))
			continue;
		if (starts_with(buffer, "end_header"))
			break;

		linhas.push_back(buffer);
	}

	//número de vértices
	{
		puts(linhas[0].c_str());
		assert(1 == sscanf(linhas[0].c_str(), " element vertex %i", &numVertices));
	}
	//propriedades
	{
		assert(verificaPropriedade(linhas[1].c_str(), "x"));
		assert(verificaPropriedade(linhas[2].c_str(), "y"));
		assert(verificaPropriedade(linhas[3].c_str(), "z"));
		assert(verificaPropriedade(linhas[4].c_str(), "nx"));
		assert(verificaPropriedade(linhas[5].c_str(), "ny"));
		assert(verificaPropriedade(linhas[6].c_str(), "nz"));
		assert(verificaPropriedade(linhas[7].c_str(), "s"));
		assert(verificaPropriedade(linhas[8].c_str(), "t"));
	}
	//número de faces
	{
		assert(1 == sscanf(linhas[9].c_str(), " element face %i", &numFaces));
	}

	//Lê todos os vértices primeiro
	Modelo::Vertices vertices;
	vertices.resize(numVertices);
	for (int i=0 ; i<numVertices ; i++)
	{
		Vertice &v = vertices[i];

		fgets(buffer, 0xFF, in);
		sscanf(buffer, "%f %f %f %f %f %f %f %f",
			&v.posicao[0], &v.posicao[1], &v.posicao[2],
			&v.normal[0], &v.normal[1], &v.normal[2],
			&v.texCoord[0], &v.texCoord[1]
		);

		v.texCoord[1] = 1.0f - v.texCoord[1];
	}

	//Agora lê os índices
	Modelo::Indices indices;
	//Só aceita triângulos
	indices.resize(3 * numFaces);
	for (int i=0 ; i<numFaces ; i++)
	{
		int nVerts;
		fgets(buffer, 0xFF, in);

		sscanf(buffer, "%i %i %i %i",
			&nVerts,
			&indices[i*3+0], &indices[i*3+1], &indices[i*3+2]
		);

		if (nVerts != 3)
			throw std::runtime_error("Nao aceito outra coisa senao triangulos!!");
	}

	Modelo *modelo = new Modelo();
	modelo->carrega(vertices, indices);

	return modelo;
}
