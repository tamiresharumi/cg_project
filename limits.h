#ifndef LIMITS_H
#define LIMITS_H

#include <cstdio>
#include <vector>
#include "objeto.h"

struct _t{
    int wWall;
    int wPosition;
};


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
        int getWallX(FILE *right, FILE *left, int defT, int defC);
        int getWallZ(FILE *front, FILE *behind, int defL, int defT);
        float inc;
        int C ,L, T, J;
        //em que parede a janela está e em que posicao
        std::vector<_t> Janelas;
        /**
        C de comprimento - tamanho em z
        L de largura - tamanho em x
        T de teto - tamanho em y
        J de janela - se maior que 0, existe
        */
};

#endif
