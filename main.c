#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegPrint.h"
#include "RegBusca.h"



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

    FILE *fp = fopen("arquivoB.bin","rb");

    if(fp == NULL){
        printf("Problema na abertura");
        return 1;
    }

    REG_DADOS* reg1 = ler_regDados(fp);

    //printt_reg(reg1);

    //imprimir_registros(reg1);

    //ler_linha_busca(NULL);
    print_registros(fp);

    fclose(fp);

    

}