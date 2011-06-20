#include "limits.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define TESTE 1


Limits::Limits(std::vector<Objeto*> &objetos){
    inc = 0.5;
    readFile("room.model", objetos);
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


int Limits::readFile(const char *filename, std::vector<Objeto*> &objetos)
{
    FILE* f = fopen(filename, "r+");
    FILE* t;
    int i=0;
    char testes[15];
    bool l = 0;

	char map[0xff][0xff];
	int direction;
	int smaller;

    C = 0;
    L = 0;

    if(!f) printf("Arquivo nao foi aberto!\n");
    printf("A Harumi é fofinha! xD\n\n");
    char linha[255];
    while (!feof(f))
	{
        if (fgets(linha, 255, f))
        {
            if (linha[0] == '#')
            {
                if (linha[1] == 'T')
                    sscanf(linha, "#T %d", &T);

                else if (linha[1] == 'J')
                    sscanf(linha, "#J %d", &J);
            }
            else if ((linha[0] >= 'A')&&(linha[0] <= 'Z'))
			{
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
        i++;
	}
	rewind(f);
	while (fgets(map[0], 0xff, f))
	{
		if (map[0][0] == 'P')
			break;
	}
	for (int i=1 ; i<=L ; ++i)
	{
		fgets(map[i], 0xff, f);
	}
	fclose(f);

	//testa se é necessário gerar os pes
    for(i=1;i<=6;i++){
        sprintf(testes, "walls/p%i.obj", i);
        t = fopen(testes, "r+");
        if(t==0)
            i=8;
        else
            fclose(t);
    }
    printf("i is %d\n", i);
    if(i>7){
        getFloor(0, "walls/p1.obj");
        getFloor(T, "walls/p6.obj");
        getWall(0,"walls/p3.obj", "walls/p4.obj");  //parede 3: direita; parede 4: esquerda
        getWall(1, "walls/p2.obj", "walls/p5.obj"); //parede 2: frente; parede 5: atras
    }

	//carrega os modelos do mapa
	for (int x=0 ; x<C ; ++x)
	{
		for (int y=0 ; y<L ; ++y)
		{
			float xx = x * (C-2.0f)/(C-1.0f) + (2-C)/2.0f;
			float yy = y * (L-2.0f)/(L-1.0f) + (2-L)/2.0f;

            smaller = y;
            direction = 0;
            if(smaller > x)
            {
                direction = 1;
                smaller = x;
            }
            if(smaller > (L-y))
            {
                direction = 2;
                smaller = L-y;
            }
            if(smaller > (C-x))
            {
                direction = 3;
                smaller = 1;
            }


			if ((map[y][x] == 'S')&&((map[y-1][x] != 'S')||(map[y][x-1] != 'S'))) //sofa
			{
                if(map[y][x+1] == 'S')//testa dois lugares
                {
                    if(2*y<L)
                        direction = 0;
                    else direction = 2;

                    if(map[y][x+2] == 'S') //testa tres lugares
                    {
                        //desenha sofá de 3 lugares
                    }
                    else
                    {
                        printf("(2*%d) < %d\ndirection is %d\n", y, L, direction);
                        objetos.push_back(new Objeto("models/sofa2.obj", Transformacao(90*direction, xx, 0, yy), "leathers/fur_leather.jpg"));
                        x+=2;
                    }
                }
                else if (map[y+1][x] == 'S') //testa dois lugares
                {
                    if(2*x<C)
                        direction = 1;
                    else direction = 3;
                    if(map[y+2][x] == 'S') //testa tres lugares
                    {
                        //desenha sofa 3 lugares virado pra algum lugar
                    }
                    else
                    {
                        printf("(2*%d) < %d\ndirection is %d\n", y, L, direction);
                        objetos.push_back(new Objeto("models/sofa2.obj", Transformacao(90*direction, xx, 0, yy), "leathers/fur_leather.jpg"));
                        x+=2;
                    }
                }
                else
                {
                    printf("sofa1: direction is %d\n", direction);
                    objetos.push_back(new Objeto("models/sofa1.obj", Transformacao(90*direction, xx, 0, yy), "leathers/red_velvet_leather.jpg"));
                }

			}

			if (map[y][x] == 'M')
				objetos.push_back(new Objeto("models/t_table.obj", Transformacao(0, xx, 0, yy)));
			if (map[y][x] == 'L')
				objetos.push_back(new Objeto("models/floor_lamp.obj", Transformacao(0, xx, 0, yy)));
            if ( (map[y][x] == 'J') && ( (map[y-1][x] != 'J') || (map[y][x-1] != 'J') ) ){
                _t janela;
                if(x == L)
                {
                    //parede da direita
                    janela.wPosition = y;
                    janela.wWall = 2;
                }
                else if(x == 0)
                {
                    //parede da esquerda
                    janela.wPosition = y;
                    janela.wWall = 4;
                }
                else {
                    janela.wPosition = x;
                    if(y == 0)
                        janela.wWall = 1;
                    else
                        janela.wWall = 3;
                }


                Janelas.push_back(janela);
            }
		}
	}
	return 1;
}


int Limits::getFloor(float y, const char* arq){
//comentario
    float i, j;
    float px = (C/inc)+1;
    float pz = (L/inc)+1;
    if((fmod(C, inc) != 0)||(fmod(L, inc) != 0)){
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

    FILE *a = fopen(arqr, "w+");
    FILE *b = fopen(arql, "w+");
    if(J>0){
        for(int i=0;i<Janelas.size();i++){
            if(Janelas[i].wWall == 1){
            }
        }
    }

    if(!xz){
        getWallX(a, T, L, +C/2);
        getWallX(b, T, L, -C/2);
    }
    else {
		getWallZ(a, C, T, L/2);
		getWallZ(b, C, T, -L/2);
	}
	fclose(a);
    fclose(b);
	return 1;
}


int Limits::getWallX(FILE* wall, float defT, float defC, float L){

    float i, j;
    float T = defT;
    float C = defC;
    float py = (T/inc)+1;
    float pz = (C/inc)+1;

    if((fmod(T, inc) != 0)||(fmod(C, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }

    for(i=(T/2); i>=(-T/2); i-=inc){
        for(j=-(C/2.); j<=(C/2.); j+=inc){
            fprintf(wall, "v %5.2f %5.2f %5.2f\n", L, (T/2.)+i, j);
        }
    }

    fprintf(wall, "vt 0 0\n");
    fprintf(wall, "vt 0 1\n");
    fprintf(wall, "vt 1 0\n");
    fprintf(wall, "vt 1 1\n");

    if (L < 0)
        fprintf(wall, "vn 1 0 0\n");
    else
        fprintf(wall, "vn -1 0 0\n");

    for(i=0;i<py-1;i++){
        for(j=0;j<pz-1;j++){
            if(L < 0)
                fprintf(wall, "f %.0f/4/1 %.0f/2/1 %.0f/3/1\n",
                    (pz*i)+(j+1), (i*pz) + j + 2, (i+1)*pz + (j+1));
            else
                fprintf(wall, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (pz*i)+(j+1), (i+1)*pz + j + 1, (i*pz) + (j+2));
        }
    }

        for(i=0;i<py-1;i++){
            for(j=1;j<pz;j++){
                if (L < 0)
                    fprintf(wall, "f %.0f/2/1 %.0f/1/1 %.0f/3/1\n",
                        (pz*i)+(j+1), (i+1)*pz +(j+1), (i+1)*pz + j);
                else fprintf(wall, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                        (pz*i)+(j+1), (i+1)*pz + j, (i+1)*pz + j+1);
        }
    }
	return 1;
}


int Limits::getWallZ(FILE* front, float defL, float defT, float C){

    float i, j;
    float L = defL;
    float T = defT;

    float px = (L/inc)+1;
    float py = (T/inc)+1;


    if((fmod(T, inc) != 0)||(fmod(L, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }

    for(i=T/2.; i>=(-T/2.); i-=inc){
        for(j=-(L/2.); j<=(L/2.); j+=inc){
            fprintf(front, "v %5.2f %5.2f %5.2f\n", j, i + T/2., C);
        }
    }
    fprintf(front, "vt 0 0\n");
    fprintf(front, "vt 0 1\n");
    fprintf(front, "vt 1 0\n");
    fprintf(front, "vt 1 1\n");

    if(C < 0)
        fprintf(front, "vn 0 0 1\n");
    else fprintf(front, "vn 0 0 -1\n");

    for(i=0;i<py-1;i++){
        for(j=0;j<px-1;j++){
            if(C < 0)
                fprintf(front, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (px*i)+(j+1), (i+1)*px + (j+1), (i*px) + (j+2));
            else fprintf(front, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (px*i)+(j+1), (i+1)*px + j + 1, (i*px) + (j+2));
        }
    }
    for(i=0;i<py-1;i++){
        for(j=1;j<px;j++){
            if(C < 0)
                fprintf(front, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                   (px*i)+(j+1), (i+1)*px +j, (i+1)*px + (j+1));
            else
                fprintf(front, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                   (px*i)+(j+1), (i+1)*px + j, (i+1)*px + j+1);
        }
    }
	return 1;
}

