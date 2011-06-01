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

float Limits::getT(void){
    return T;
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
    getWall(0,"p3.obj", "p4.obj");
}


int Limits::getFloor(float y, const char* arq){

    float i, j;

    float px = (L/inc)+1;
    float pz = (C/inc)+1;


    printf("px|%f\npz|%f\n\n", px, pz);
    if((fmod(L, inc) != 0)||(fmod(C, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }
    FILE* f = fopen(arq, "w+");
    FILE* x = fopen("log_for.log", "w+");
    for(i=-(C/2.); i<=(C/2.); i+=inc){
        for(j=-(L/2.); j<=(L/2.); j+=inc){
            fprintf(f, "v %5.2f %5.2f %5.2f\n", i, y, j);
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
    printf("= Teste =\nNo quadrado de cima:\ni vai de 0 a %.0f\nj vai de 0 a %.0f\n\n", (pz-2), (px-2));
#endif

    for(i=0;i<(pz-1);i++){
        for(j=0;j<(px-1);j++){
            fprintf(f, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (i*px)+(j+1), ((i+1)*px + (j+1)), (i*px)+(j+2));
            fprintf(x, "i=%.1f  j=%.1f  v=%.1f %.1f %.1f\n", i, j,
                    (i*px)+(j+1), ((i+1)*px + (j+1)), (i*px)+(j+2));
        }
    }
    for(i=0;i<(pz-1);i++){
        for(j=1;j<px;j++){
            fprintf(f, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                      (i*px)+(j+1), ((i+1)*px + j), ((i+1)*px)+(j+1));
        }
    }
    fclose(f);
    fclose(x);
    return 1;
}


int Limits::getWall(bool xz, const char* arqr, const char *arql){

   float i, j;
       //TODO verificar arquivo e comparar dados com o programa atual
    FILE *a = fopen(arqr, "w+");
    FILE *b = fopen(arql, "w+");
    if(!xz){
        getWallX(a, b);
    }
    //else getWallZ(v, a);
    fclose(a);
    fclose(b);
}

int Limits::getWallX(FILE* right, FILE *left){

    float i, j;
    float py = (T/inc)+1;
    float pz = (C/inc)+1;
#if TESTE
    printf("py|%f\npz|%f\n\n", py, pz);
    printf("= Teste =\nNo quadrado de cima:\ni vai de 0 a %.0f\nj vai de 0 a %.0f\n\n", (py-2), (pz-2));
#endif

    if((fmod(T, inc) != 0)||(fmod(C, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }
    for(i=T; i>=0; i-=inc){
        for(j=-(C/2.); j<=(C/2.); j+=inc){
            fprintf(left, "v %5.2f %5.2f %5.2f\n", -(C/2.), i, j);
            fprintf(right, "v %5.2f %5.2f %5.2f\n", C/2., i, j);
        }
    }
    fprintf(right, "vt 0 0\n");
    fprintf(right, "vt 0 1\n");
    fprintf(right, "vt 1 0\n");
    fprintf(right, "vt 1 1\n");
    fprintf(left, "vt 0 0\n");
    fprintf(left, "vt 0 1\n");
    fprintf(left, "vt 1 0\n");
    fprintf(left, "vt 1 1\n");

    fprintf(right, "vn -1 0 0\n");
    fprintf(left, "vn 1 0 0\n");

    for(i=0;i<py-1;i++){
        for(j=0;j<pz-1;j++){
            fprintf(left, "f %.0f/4/1 %.0f/2/1 %.0f/3/1\n",
                    (pz*i)+(j+1), (i*pz) + j + 2, (i+1)*pz + (j+1));
            fprintf(right, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (pz*i)+(j+1), (i+1)*pz + j + 1, (i*pz) + (j+2));
        }
    }
}
