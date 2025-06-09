#include <stdio.h>
#include "RegRW.h"
#include <string.h>
#include <stdlib.h>

bool CSV_to_BIN(char* nomecsv, char* nomeBin){
    FILE *fp = fopen(nomecsv,"r");
    FILE* bin = fopen(nomeBin,"wb");
    char linha[300];
    if(bin == NULL || fp == NULL){
        return false;
    }

    fgets(linha,sizeof(linha),fp);//CONSOME A PRIMEIRA LINHA DO ARQUIVO CSV
    fseek(bin,276,SEEK_SET); //DESLOCA O BINARIO PARA POSICAO LOGO APOS O HEADER

    REG * reg = criar_reg();
    HEADER* h = criar_header();
    
    
    escrever_registros(fp,bin,reg,h);

    set_proxByteOffset(h,ftell(bin));
    fseek(bin,0,SEEK_SET);
    fseek(fp,0,SEEK_SET);
    escrever_cabecalho(fp,bin,h);

    fclose(bin);
    free(h);
    fclose(fp);
    free(reg);

    return true;
}
