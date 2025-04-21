#include <stdio.h>
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

REG_DADOS criar_regDados(){
    REG_DADOS reg;

    reg.removido = '0';
    reg.tamanhoRegistro = 25;
    reg.prox = -1;

    return reg;
}

void add_lixo(int tam, char *vet){
    for(int i = 0;i<tam;i++){
        vet[i] = '$';
    }
}

REG_DADOS escrever_regDados(REG_DADOS reg){
    FILE * fp = fopen("arquivoB.bin", "a+");

    if(fp == NULL){
        printf("Erro ao abrir o arquivo");
        return; 
    }
    
    fwrite(&reg.removido,sizeof(char),1,fp);
    fwrite(&reg.tamanhoRegistro,sizeof(int),1,fp);
    fwrite(&reg.prox,sizeof(long int),1,fp);
    fwrite(&reg.idAttack,sizeof(int),1,fp);
    fwrite(&reg.year,sizeof(int),1,fp);
    fwrite(&reg.financialLoss,sizeof(float),1,fp);

    char aux = reg.country[0];
    fclose(fp);

    
}

void ler_campos_variaveis(FILE *fp,REG_DADOS reg){
    char aux;
    
    fread(&aux,sizeof(char),1,fp);
    int i = 0;
    if(aux == '1'){
        do{
            fread(&reg.country[i],sizeof(char),1,fp);
        }
        while(reg.country[i++]!='|');
        reg.country[i-1] = '\0'; 
    }
          
    
    fread(&aux,sizeof(char),1,fp);
    i = 0;
    if(aux == '2'){
        do{
            fread(&reg.attackType[i],sizeof(char),1,fp);
        }
        while(reg.attackType[i++]!='|'); 
        reg.attackType[i-1] = '\0';     
    }
    fread(&aux,sizeof(char),1,fp);
    i=0;
    if(aux == '3'){
        do{
            fread(&reg.targetIndustry[i],sizeof(char),1,fp);
        }
        while(reg.targetIndustry[i++]!='|');
        reg.targetIndustry[i-1] = '\0';      
    }
    fread(&aux,sizeof(char),1,fp);
    i=0;
    if(aux == '4'){
        do{
            fread(&reg.defenseMechanism[i],sizeof(char),1,fp);
        }
        while(reg.defenseMechanism[i++]!='|');
        reg.defenseMechanism[i-1] = '\0';      
    }
     
}

REG_DADOS ler_regDados(FILE *fp){
    REG_DADOS reg = criar_regDados();
    fread(&reg.removido,sizeof(char),1,fp);
    fread(&reg.tamanhoRegistro,sizeof(int),1,fp);
    fread(&reg.prox,sizeof(long int),1,fp);
    fread(&reg.idAttack,sizeof(int),1,fp);
    fread(&reg.year,sizeof(int),1,fp);
    fread(&reg.financialLoss,sizeof(float),1,fp);
    
    ler_campos_variaveis(fp, reg);

    return reg;
}