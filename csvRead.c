#include <stdio.h>
#include "RegRW.h"
#include <string.h>
#include <stdlib.h>

bool CSV_to_BIN(char* nomecsv, char* nomeBin){
    FILE *fp = fopen(nomecsv,"r");
    FILE* bin = fopen(nomeBin,"wb");
    //char linha[300];
    if(bin == NULL || fp == NULL){
        return false;
    }
    REG * reg = criar_reg();
    HEADER* h = criar_header();
    
    escrever_cabecalho(fp,bin,h);
    escrever_registros(fp,bin,reg);

    fclose(bin);
    free(h);
    fclose(fp);
    free(reg);

    return true;
}
