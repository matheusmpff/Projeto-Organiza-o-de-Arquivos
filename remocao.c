#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"

bool compara_campos(REG* reg, char* campos, char*valores, int tamanho){
    for(int i = 0 ;i < tamanho; i++){
        if(strcmp(campos[i],"country") == 0){}
        if(strcmp(campos[i],"attackType") == 0){}
        if(strcmp(campos[i],"financialLoss") == 0){}
        if(strcmp(campos[i],"defenseMechanism") == 0){}
        if(strcmp(campos[i],"targetIndustry") == 0){}
        if(strcmp(campos[i],"") == 0){}
        if(strcmp(campos[i],"idAttack") == 0){}
    }
}

void remover_registro(char * nomebin, char* campos, char* valores, int tamanho){
    FILE * bin = fopen(nomebin,"rb+");
    if(bin == NULL){
        printf("Erro no processamento do arquivo\n");
        return;
    }

    long int fim = fseek(bin,0,SEEK_END);//encontra o fim do arquivo para o loop
    fseek(bin,0,SEEK_SET);//COLOCA O PONTEIRO NO INICIO DO HEADER
    REG* reg;
    HEADER * h = criar_header();
    ler_header(bin,h);
    while(ftell(bin) != fim){

        reg = ler_registro(bin,h);
        
        compara_campos(reg,campos,valores, tamanho);
        free(reg);
    }


    fclose(bin);

}