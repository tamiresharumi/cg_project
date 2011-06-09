#ifndef LIMITS_H
#define LIMITS_H

#include <cstdio>
#include <vector>
#include "objeto.h"

class Limits
{
    public:
        Limits();
        void setInc(float nInc);
        float getT(void);
        float getC(void);
        float getL(void);
		void readModels(std::vector<Objeto*> &objetos);
    private:
        int readFile(const char *filename);
        int getFloor(float y, const char* arq);
        int getWall(bool xz, const char* arqr, const char *arql);
        int getWallX(FILE *right, FILE *left);
        int getWallZ(FILE *front, FILE *behind);
        float inc;
        int C ,L, T;
        /**
        C de comprimento - tamanho em z
        L de largura - tamanho em x
        T de teto - tamanho em y
        */
};

#endif
