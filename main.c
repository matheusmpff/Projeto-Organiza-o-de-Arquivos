#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegPrint.h"



int main(){
    
    REGPARAMS params;

    params.removido = '0';
    params.tamanhoRegistro = 57;
    params.prox = 13;
    params.idAttack = 14;
    params.year = 2001;
    params.financialLoss = 17;
    strcpy(params.country,"1Brasil|"); // keyword igual a 1
    strcpy(params.attackType,"2Bombardeio|") ;//keyword igual a 2
    strcpy(params.targetIndustry,"3BTG|");//keyword igual a 3
    strcpy(params.defenseMechanism,"4drone|");//keyword igual a 4

    REG_DADOS* reg = criar_regDados(params);

    escrever_regDados(reg);

    FILE *fp = fopen("binario.bin","rb");

    if(fp == NULL){
        printf("Problema na abertura");
        return 1;
    }

    printf("cheguei aqui");
    REG_DADOS* reg1 = ler_regDados(fp);

    printt_reg(reg1);


    

    fclose(fp);

    

}