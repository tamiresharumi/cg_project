#include "limits.h"
#include <stdio.h>
#include <math.h>
#define TESTE 1


Limits::Limits(){
    inc = 0.5;
    readFile("room.model");
}

void Limits::setInc(float nInc){
    inc = nInc;
}

int Limits::readFile(const char *filename)
{
    FILE* f = fopen(filename, "r+");

    if(!f) printf("Arquivo nao foi aberto!\n");
    printf("xD\n\n");
    char linha[255];
    while (!feof(f))
	{
        if (fgets(linha, 255, f))
        {
            if (linha[0] == '#' && linha[1] == 'T')
                sscanf(linha, "#T %d", &T);
            else if (linha[0] == '#' && linha[1] == 'C')
                sscanf(linha, "#C %d", &C);
            else if (linha[0] == '#' && linha[1] == 'L')
                sscanf(linha, "#L %d", &L);
        }
	}
	fclose(f);
    printf("T|%d\nC|%d\nL|%d\n\n", T, C, L);
    getFloor(0, "p0.obj");
    getFloor(T, "p6.obj");
}


int Limits::getFloor(float y, const char* arq){

    float i, j;
    float px = (L/inc)+1;
    float pz = (C/inc)+1;
#if TESTE
    printf("px|%f\npz|%f\n\n", px, pz);
#endif

    if((fmod(L, inc) != 0)||(fmod(C, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }
    FILE* f = fopen(arq, "w+");
    for(i=-(L/2.); i<=(L/2.); i+=inc){
        for(j=-(C/2.); j<=(C/2.); j+=inc){
            fprintf(f, "v %5.2f %5.2f %5.2f\n", j, y, i);
        }
    }
    fprintf(f, "vt 0 0\n");
    fprintf(f, "vt 0 1\n");
    fprintf(f, "vt 1 0\n");
    fprintf(f, "vt 1 1\n");

    if(y!=0)
        fprintf(f, "vn 0 -1 0\n");
    else
        fprintf(f, "vn 0 1 0\n");

//faces do quadrado de cima
#if TESTE
    printf("= Teste =\nNo quadrado de cima:\ni vai de 0 a %.0f\nj vai de 0 a %.0f\n\n", (pz-1), (px-1));
#endif

    for(i=0;i<(pz-1);i++){
        for(j=0;j<(px-1);j++){
            fprintf(f, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (i*pz)+(j+1), ((i+1)*pz + (j+1)), (i*pz)+(j+2));
        }
    }
    for(i=0;i<(pz-1);i++){
        for(j=1;j<px;j++){
            fprintf(f, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                      (i*pz)+(j+1), ((i+1)*pz + j), ((i+1)*pz)+(j+1));
        }
    }
    fclose(f);
    return 1;
}

//int Limits::getWall(bool xz, float v, const char* arq){
//
//    float i, j;
//    if(xz){  //fixamos z; paredes
//        float px = (L/inc)+1;
//        float x;
//    }
//    else
//        float pz = (C/inc)+1;
//    float py = (T/inc)+1;
//
//#if TESTE
//    printf("px|%f\npz|%f\n\n", px, pz);
//#endif
//
//    if((fmod(L, inc) != 0)||(fmod(C, inc) != 0)){
//        printf("Incremento incompativel!\n");
//        return 0;
//    }
//    FILE* f = fopen(arq, "w+");
//    for(i=-(L/2.); i<=(L/2.); i+=inc){
//        for(j=-(C/2.); j<=(C/2.); j+=inc){
//            fprintf(f, "v %5.2f %5.2f %5.2f\n", j, y, i);
//        }
//    }
//    fprintf(f, "vt 0 0\n");
//    fprintf(f, "vt 0 1\n");
//    fprintf(f, "vt 1 0\n");
//    fprintf(f, "vt 1 1\n");
//}
