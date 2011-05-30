#ifndef LIMITS_H
#define LIMITS_H

class Limits
{
    public:
        Limits();
        void setInc(float nInc);
    private:
        int readFile(const char *filename);
        int getFloor(float y, const char* arq);
        int getWall(bool xz, float v, const char* arq);

        float inc;
        int C ,L, T;
        /**
        C de comprimento
        L de largura
        T de teto
        */
};

#endif
