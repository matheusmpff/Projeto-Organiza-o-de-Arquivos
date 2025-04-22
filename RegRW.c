#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "RegRW.h"

struct header{
    char status;
    long int topo;
    long int proxByteOffset;
    int nroReqArq;
    int nroReqRem;
    char descreveIdentificador [24];
    char descreveYear[28];
    char descreverFinancialLoss[29];
    char codDescreveCountry;
    char descreveCountry[27];
    char codDescreveType;
    char descreveType[39];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[39];
    char codDescreveDefense;
    char descreveDefense[68];
};

struct reg_dados{
    char removido;
    int tamanhoRegistro;
    long int prox;
    int idAttack;
    int year;
    float financialLoss;
    char country[20]; // keyword igual a 1
    char attackType[20];//keyword igual a 2
    char targetIndustry[20];//keyword igual a 3
    char defenseMechanism[20];//keyword igual a 4
};

HEADER create_header(){

    HEADER h;
    h.status = 0;
    h.topo = 0;
    h.proxByteOffset = 0;
    h.nroReqArq = 0;
    h.nroReqRem = 0;
    h.codDescreveCountry = '2';
    h.codDescreveType= '2';
    h.codDescreveTargetIndustry = '3';
    h.codDescreveDefense = '4';
    return h;
}

    REG_DADOS* criar_regDados(REGPARAMS params){
        REG_DADOS* reg;

        reg = (REG_DADOS*) malloc(sizeof(REG_DADOS)*1);

        reg->removido = params.removido; 
        reg->tamanhoRegistro = params.tamanhoRegistro;
        reg->prox = params.prox;
        reg->year = params.year;
        reg->idAttack = params.idAttack;
        reg->financialLoss = params.financialLoss;
        strcpy(reg->targetIndustry,params.targetIndustry);
        strcpy(reg->defenseMechanism,params.defenseMechanism);
        strcpy(reg->attackType,params.attackType);
        strcpy(reg->country,params.country);

        return reg;
    }

void add_lixo(int tam, char *vet){
    for(int i = 0;i<tam;i++){
        vet[i] = '$';
    }
}

bool escrever_regDados(REG_DADOS *reg){
    FILE * fp = fopen("arquivoB.bin", "a+");

    if(fp == NULL){
        printf("Erro ao abrir o arquivo");
         return false;
    }
    
    fwrite(&reg->removido,sizeof(char),1,fp);
    fwrite(&reg->tamanhoRegistro,sizeof(int),1,fp);
    fwrite(&reg->prox,sizeof(long int),1,fp);
    fwrite(&reg->idAttack,sizeof(int),1,fp);
    fwrite(&reg->year,sizeof(int),1,fp);
    fwrite(&reg->financialLoss,sizeof(float),1,fp);
    
    char aux = '|';

    char flag = '1';
    if(reg->country[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->country[i]!='\0';i++){
        fwrite(&reg->country[i],sizeof(char),1,fp);
    }
    if(reg->country[0]!='\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }
    
    flag = '2';
    if(reg->attackType[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->attackType[i]!='\0';i++){
        fwrite(&reg->attackType[i],sizeof(char),1,fp);
    }
    if(reg->attackType[0] != '\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }
    
    flag = '3';
    if(reg->targetIndustry[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->targetIndustry[i]!='\0';i++){
        fwrite(&reg->targetIndustry[i],sizeof(char),1,fp);
    }
    if(reg->targetIndustry[0]!= '\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }
   
    flag = '4';
    if(reg->defenseMechanism[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->defenseMechanism[i]!='\0';i++){
        fwrite(&reg->defenseMechanism[i],sizeof(char),1,fp);
    }
    if(reg->defenseMechanism[0]!='\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }

    fclose(fp);
    return true;
    
}

void ler_campos_variaveis(FILE *fp,REG_DADOS *reg){
    char aux;
    
    fread(&aux,sizeof(char),1,fp);
    int i = 0;
    if(aux == '1'){
        do{
            fread(&reg->country[i],sizeof(char),1,fp);
            i++;
        }
        while(reg->country[i]!='|');
        reg->country[i-1] = '\0'; 
    }
    
    i = 0;
    if(aux == '2'){
        do{
            fread(&reg->attackType[i],sizeof(char),1,fp);
            i++;
        }
        while(reg->attackType[i]!='|'); 
        reg->attackType[i-1] = '\0';
        fread(&aux,sizeof(char),1,fp);    
    }
    
    i=0;
    if(aux == '3'){
        do{
            fread(&reg->targetIndustry[i],sizeof(char),1,fp);
            i++;
        }
        while(reg->targetIndustry[i]!='|');
        reg->targetIndustry[i-1] = '\0';
        fread(&aux,sizeof(char),1,fp);      
    }
    
    i=0;
    if(aux == '4'){
        do{
            fread(&reg->defenseMechanism[i],sizeof(char),1,fp);
            i++;
        }
        while(reg->defenseMechanism[i]!='|');
        reg->defenseMechanism[i-1] = '\0';      
    }
     
}

REG_DADOS* ler_regDados(FILE *fp){
    REGPARAMS params;
    REG_DADOS *reg = criar_regDados(params);
    fread(&reg->removido,sizeof(char),1,fp);
    fread(&reg->tamanhoRegistro,sizeof(int),1,fp);
    fread(&reg->prox,sizeof(long int),1,fp);
    fread(&reg->idAttack,sizeof(int),1,fp);
    fread(&reg->year,sizeof(int),1,fp);
    fread(&reg->financialLoss,sizeof(float),1,fp);
    
    
    ler_campos_variaveis(fp, reg);

    return reg;
}

void printt_reg(REG_DADOS* reg){

    printf("removido: %c\n",reg->removido);
    printf("tamanhoReg: %d\n",reg->tamanhoRegistro);
    printf("prox: %ld\n",reg->prox);
    printf("idAttack: %d\n",reg->idAttack);
    printf("year: %d\n",reg->year);
    printf("financial loss: %f\n",reg->financialLoss);
    printf("country: %s\n",reg->country);
    printf("attacktype: %s\n",reg->attackType);
    printf("defense: %s\n",reg->defenseMechanism);
    printf("target: %s\n",reg->targetIndustry);
}