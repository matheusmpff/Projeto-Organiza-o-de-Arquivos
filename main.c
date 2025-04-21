#include <stdio.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegPrint.h"

int main(){
    FILE *fp = fopen("binario.bin","wb");

    if(fp == NULL){
        printf("Problema na abertura");
        return 1;
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

    for(int i = 0; country[i] != '\0';i++){
        fwrite(&country[i],sizeof(char),1,fp);
    }
    for(int i = 0; attackType[i] != '\0';i++){
        fwrite(&attackType[i],sizeof(char),1,fp);
    }
    for(int i = 0; targetIndustry[i] != '\0';i++){
        fwrite(&targetIndustry[i],sizeof(char),1,fp);
    }
    for(int i = 0; defenseMechanism[i] != '\0';i++){
        fwrite(&defenseMechanism[i],sizeof(char),1,fp);
    }
    fclose(fp);

    fp = fopen("binario.bin","rb");

    if(fp == NULL){
        printf("Problema na abertura");
        return 1;
    }

    


    char removido1;
    int tamanhoRegistro1;
    long int prox1;
    int idAttack1;
    int year1;
    float financialLoss1;
    char country1[20] ="1"; // keyword igual a 1
    char attackType1[20];//keyword igual a 2
    char targetIndustry1[20];//keyword igual a 3
    char defenseMechanism1[20];//keyword igual a 4

}