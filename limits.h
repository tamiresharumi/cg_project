#ifndef LIMITS_H
#define LIMITS_H

#include <cstdio>
#include <vector>
#include "objeto.h"

class Limits
{
    public:
        Limits(std::vector<Objeto*> &objetos);
        void setInc(float nInc);
        float getT(void);
        float getC(void);
        float getL(void);
    private:
        int readFile(const char *filename, std::vector<Objeto*> &objetos);
        int getFloor(float y, const char* arq);
        int getWall(bool xz, const char* arqr, const char *arql);
        int getWallX(FILE *right, FILE *left);
        int getWallZ(FILE *front, FILE *behind);
        float inc;
        int C ,L, T, J;
        /**
        C de comprimento - tamanho em z
        L de largura - tamanho em x
        T de teto - tamanho em y
        J de janela - se maior que 0, existe
        */
};

#endif
