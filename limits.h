#include <stdio.h>
#ifndef LIMITS_H
#define LIMITS_H

class Limits
{
    public:
        Limits();
        void setInc(float nInc);
        float getT(void);
    private:
        int readFile(const char *filename);
        int getFloor(float y, const char* arq);
        int getWall(bool xz, const char* arqr, const char *arql);
        int getWallX(FILE *right, FILE *left);
        float inc;
        int C ,L, T;
        /**
        C de comprimento
        L de largura
        T de teto
        */
};

#endif
