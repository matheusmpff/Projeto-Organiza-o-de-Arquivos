#include <stdio.h>
#include "RegRW.h"
#include <string.h>
#include <stdlib.h>

bool csv_to_bin(){
    FILE *fp;
    FILE* bin;
    HEADERPARAMS params_h;

    fp = fopen("exemplos.csv","r");
    bin = fopen("arquivoB.bin","ab");
    

    if(fp == NULL || bin == NULL){
        printf("Problema na abertura do arquivo");
        return false;
    }
    fseek(fp,0,SEEK_END);
    long int fim_csv = ftell(fp);
    fseek(fp,0,SEEK_SET);
    
    params_h.topo = -1;
    params_h.proxByteOffset = 0;
    params_h.nroReqArq = 0;
    params_h.nroReqRem = 0;
    
    fread(params_h.descreveIdentificador,sizeof(char),24,fp);
    params_h.descreveIdentificador[23] = '\0';
    fread(params_h.descreveYear,sizeof(char),28,fp);
    params_h.descreveYear[27] = '\0';
    fread(params_h.descreverFinancialLoss,sizeof(char),29,fp);
    params_h.descreverFinancialLoss[28] = '\0';
    fread(params_h.descreveCountry,sizeof(char),27,fp);
    params_h.descreveCountry[27] = '\0';
    fread(params_h.descreveType,sizeof(char),39,fp);
    params_h.descreveType[38] = '\0';
    fread(params_h.descreveTargetIndustry,sizeof(char),39,fp);
    params_h.descreveTargetIndustry[38]='\0';
    fread(params_h.descreveDefense,sizeof(char),68,fp);
    params_h.descreveDefense[67]='\0';
    
    
    fseek(bin,276,SEEK_SET);



    while(fp != fim_csv){ 
        REGPARAMS aux;
        aux.removido = '0';
        aux.tamanhoRegistro = 25;
        aux.prox = -1;
       
        if(fscanf(fp,"%d,",&aux.idAttack)!=1){
            aux.idAttack = -1;
        }
        if(fscanf(fp,"%d,",&aux.year)!=1){
            aux.financialLoss = -1;
        }
        if(fscanf(fp,"%f,",&aux.financialLoss)!=1){
            aux.financialLoss = -1;
        }
        fscanf("%[^,],%[^,],%[^,],%[^,]",aux.country,aux.attackType,aux.targetIndustry,aux.defenseMechanism);

        if(aux.country[0]!='\0'){
            aux.tamanhoRegistro += strlen(aux.country);
        }
        if(aux.attackType[0] != '\0'){
            aux.tamanhoRegistro += strlen(aux.attackType);
        }
        if(aux.targetIndustry[0] != '\0'){
           aux.tamanhoRegistro += strlen(aux.targetIndustry);
        }
        if(aux.defenseMechanism[0] != '\0'){
            aux.tamanhoRegistro += strlen(aux.defenseMechanism);
        }

        REG_DADOS* reg = criar_regDados(aux);
        escrever_regDados(bin,reg);
        params_h.nroReqArq +=1;


    }

    fseek(bin,0,SEEK_SET);
    HEADER* h = create_header();
    escrever_header(h,bin);
}
