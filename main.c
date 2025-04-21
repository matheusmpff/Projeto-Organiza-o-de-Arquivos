#include <stdio.h>
#include <stdbool.h>
#include "RegRW.h"


int main(){
    FILE *fp = fopen("binario.bin","w");

    if(fp == NULL){
        printf("Problema na abertura");
        return;
    }

    char removido = '0';
    int tamanhoRegistro = 12;
    long int prox = 13;
    int idAttack = 14;
    int year = 2001;
    float financialLoss = 17;
    char country[20] = "Brasil|"; // keyword igual a 1
    char attackType[20] = "Bombardeio|";//keyword igual a 2
    char targetIndustry[20] = "BTG|";//keyword igual a 3
    char defenseMechanism[20] = "drone|";//keyword igual a 4

    fwrite(&removido,sizeof(char),1,fp);
    fwrite(&tamanhoRegistro,sizeof(int),1,fp);
    fwrite(&prox,sizeof(long int),1,fp);
    fwrite(&idAttack,sizeof(int),1,fp);
    fwrite(&year,sizeof(int),1,fp);
    fwrite(&financialLoss,sizeof(float),1,fp);


    char removido1;
    int tamanhoRegistro1;
    long int prox1;
    int idAttack1;
    int year1;
    float financialLoss1;
    char country1[20]; // keyword igual a 1
    char attackType1[20];//keyword igual a 2
    char targetIndustry1[20];//keyword igual a 3
    char defenseMechanism1[20];//keyword igual a 4

}