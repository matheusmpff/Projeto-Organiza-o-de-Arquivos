#include <stdio.h>
#include <string.h>
#include "RegRW.h"
#include "RegPrint.h"

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

void imprimir_registros(REG_DADOS *r) {
    printf("IDENTIFICADOR DO ATAQUE: %d\n", r->idAttack);
    printf("ANO EM QUE O ATAQUE OCORREU: %d\n", r->year);
    printf("PAIS ONDE OCORREU O ATAQUE: %s\n", r->country);
    printf("SETOR DA INDUSTRIA QUE SOFREU O ATAQUE: %s\n", r->targetIndustry);
    printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: %s\n", r->attackType);
    printf("PREJUIZO CAUSADO PELO ATAQUE: %f\n", r->financialLoss);
    printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: %s\n", r->defenseMechanism);
    printf("\n");
}

int verificar_vazio(FILE *fp) {
    int tamanho = 0;
    int pos_atual = ftell(fp);

    //Vai para o final
    fseek(fp, 0, SEEK_END);
    tamanho = ftell(fp);

    //Volta a posicao original
    fseek(fp, pos_atual, SEEK_SET);

    return tamanho;
}

void select_registros(FILE *fp) {
    REG_DADOS r;
    char buffer;

    if(verificar_vazio == 0) {
        printf("Registro inexistente.\n");
    } else {
        while(fread(&buffer, sizeof(char), 1, fp) > 0) {
            fseek(fp, -1, SEEK_CUR);
            r = ler_regDados(fp);
            if (r.removido == '0') {
                imprimir_registros(&r);
            }
        } 
    }

    if(ferror(fp)) {
        printf("Falha no processamento do arquivo.\n");
    }
}