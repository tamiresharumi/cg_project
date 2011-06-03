#include "limits.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
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

float Limits::getC(void){
    return C;
}

float Limits::getL(void){
    return L;
}

int Limits::readFile(const char *filename)
{
    FILE* f = fopen(filename, "r+");
    bool l = 0;
    C = 0;
    L = 0;

    if(!f) printf("Arquivo nao foi aberto!\n");
    printf("xD\n\n");
    char linha[255];
    while (!feof(f))
	{
        if (fgets(linha, 255, f))
        {
            if (linha[0] == '#' && linha[1] == 'T')
                sscanf(linha, "#T %d", &T);
            else if ((linha[0] >= 'A')&&(linha[0] <= 'Z'))
                if(l)
                    L++;
                else {
                    C = (strlen(linha)) - 1;
                    printf("C = %d\n", C);
                    l = 1;
                    L++;
                }
        }
	}
	fclose(f);
    printf("T|%d\nC|%d\nL|%d\n\n", T, C, L);
    getFloor(0, "p1.obj");
    getFloor(T, "p6.obj");
    getWall(0,"p3.obj", "p4.obj");  //parede 3: direita; parede 4: esquerda
    getWall(1, "p2.obj", "p5.obj"); //parede 2: frente; parede 5: atras
}


int Limits::getFloor(float y, const char* arq){

    float i, j;

    float px = (L/inc)+1;
    float pz = (C/inc)+1;
    if((fmod(L, inc) != 0)||(fmod(C, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }
    FILE* f = fopen(arq, "w+");
    FILE* x = fopen("log_for.log", "w+");
    for(i=-(L/2.); i<=(L/2.); i+=inc){
        for(j=-(C/2.); j<=(C/2.); j+=inc){
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

    for(i=0;i<(px-1);i++){
        for(j=0;j<(pz-1);j++){
            fprintf(f, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (i*pz)+(j+1), ((i+1)*pz + (j+1)), (i*pz)+(j+2));
            fprintf(x, "i=%.1f  j=%.1f  v=%.1f %.1f %.1f\n", i, j,
                    (i*pz)+(j+1), ((i+1)*pz + (j+1)), (i*pz)+(j+2));
        }
    }
    for(i=0;i<(px-1);i++){
        for(j=1;j<pz;j++){
            fprintf(f, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                      (i*pz)+(j+1), ((i+1)*pz + j), ((i+1)*pz)+(j+1));
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
    else getWallZ(a, b);
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
            fprintf(left, "v %5.2f %5.2f %5.2f\n", -(L/2.), i, j);
            fprintf(right, "v %5.2f %5.2f %5.2f\n", L/2., i, j);
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

        for(i=0;i<py-1;i++){
            for(j=1;j<pz;j++){
                fprintf(left, "f %.0f/2/1 %.0f/1/1 %.0f/3/1\n",
                        (pz*i)+(j+1), (i+1)*pz +(j+1), (i+1)*pz + j);
                fprintf(right, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                        (pz*i)+(j+1), (i+1)*pz + j, (i+1)*pz + j+1);
        }
    }
}


int Limits::getWallZ(FILE* front, FILE *behind){

    float i, j;
    float px = (L/inc)+1;
    float py = (T/inc)+1;


    if((fmod(T, inc) != 0)||(fmod(L, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }

    for(i=T; i>=0; i-=inc){
        for(j=-(L/2.); j<=(L/2.); j+=inc){
            fprintf(front, "v %5.2f %5.2f %5.2f\n", j, i, -(C/2.));
            fprintf(behind, "v %5.2f %5.2f %5.2f\n", j, i, C/2.);
        }
    }
    fprintf(front, "vt 0 0\n");
    fprintf(front, "vt 0 1\n");
    fprintf(front, "vt 1 0\n");
    fprintf(front, "vt 1 1\n");
    fprintf(behind, "vt 0 0\n");
    fprintf(behind, "vt 0 1\n");
    fprintf(behind, "vt 1 0\n");
    fprintf(behind, "vt 1 1\n");

    fprintf(front, "vn 0 0 1\n");
    fprintf(behind, "vn 0 0 -1\n");

    for(i=0;i<py-1;i++){
        for(j=0;j<px-1;j++){
            fprintf(front, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (px*i)+(j+1), (i+1)*px + (j+1), (i*px) + (j+2));
            fprintf(behind, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (px*i)+(j+1), (i+1)*px + j + 1, (i*px) + (j+2));
        }
    }
    for(i=0;i<py-1;i++){
        for(j=1;j<px;j++){
            fprintf(front, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                   (px*i)+(j+1), (i+1)*px +j, (i+1)*px + (j+1));
            fprintf(behind, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                   (px*i)+(j+1), (i+1)*px + j, (i+1)*px + j+1);
        }
    }
}
