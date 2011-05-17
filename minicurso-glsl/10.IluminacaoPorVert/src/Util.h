#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>

void desenha_cubo(float meioLado);
void desenha_full_quad();

unsigned compilaShader(const char *arquivo, GLenum tipo);
unsigned criaPrograma(const char *arquivoVert, const char *arquivoFrag);

#endif
