#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "palavra.h"


palavra* palavra_cria(const char* categoria, const double* vetor,const char*palavra1,int size){
    palavra* new = malloc(sizeof(palavra));
    strcpy(new->categoria, categoria);
    strcpy(new->palavra,palavra1);
    for(int i = 0; i < size;i++){
        new->vetor[i] = vetor[i];
    }
    return new;
}

void palavra_libera(palavra* palavra1){
    free(palavra1);
}

double cosin(palavra* palavra1,palavra* palavra2,int size){
    return 1-(multvetor(palavra1,palavra2,size)/(normavetor(palavra1,size)*normavetor(palavra2,size)));
}

double dist_euclid(palavra* palavra1,palavra* palavra2,int size){
    double temp;
    double temp1 = 0;
    double *v = palavra1->vetor;
    double *u = palavra2->vetor;
    for(int i = 0; i < size; i++){
        temp = v[i]-u[i];
        temp1 += pow(temp,2);
    }
    return sqrt(temp1);
}

double normavetor(palavra* palavra1,int size){
    double temp = 0;
    double* v = palavra1->vetor;
    for(int i = 0;i<size;i++){
        temp+= pow(v[i],2);
    }
    return sqrt(temp);

}

double multvetor(palavra* palavra1,palavra* palavra2,int size){
    double temp = 0;
    double *v = palavra1->vetor;
    double *u = palavra2->vetor;
    for(int i = 0;i<size;i++){
        temp += v[i]*u[i];
    }
    return temp;
}

int norma_compara(palavra* palavra1,palavra* palavra2,int size){
    double norma1 = normavetor(palavra1,size);
    double norma2 = normavetor(palavra2,size);
    if (norma1 > norma2) return 1;
    else if (norma1 < norma2) return -1;
    return 0;
}

int palavra_compara(palavra* palavra1,palavra* palavra2,int size){
    return strcmp(palavra1->palavra,palavra2->palavra);
}