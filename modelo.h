#ifndef MODELO_H
#define MODELO_H

class Modelo
{
	public:
		float *verts;
		float *norms;
		float *tex;
		int *faces;
		int numFaces;
		int numVerts;

		int carrega(const char *arquivo);
        ~Modelo();
		void desenha();
};

#endif

