#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RegRW.h"
#include "RegPrint.h"

void imprimir_registros(REG_DADOS *r) {
    printf("IDENTIFICADOR DO ATAQUE: %d\n", get_idAttack(r));
    printf("ANO EM QUE O ATAQUE OCORREU: %d\n", get_year(r));
    printf("PAIS ONDE OCORREU O ATAQUE: %s\n", get_country(r));
    printf("SETOR DA INDUSTRIA QUE SOFREU O ATAQUE: %s\n", get_targetIndustry(r));
    printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: %s\n", get_attackType(r));
    printf("PREJUIZO CAUSADO PELO ATAQUE: %f\n", get_financialLoss(r));
    printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: %s$$\n", get_defenseMechanism(r));
}

int verificar_vazio(FILE *fp) {
    int tamanho = 0;
    int pos_atual = ftell(fp);

    //Vai para o final
    fseek(fp, 0, SEEK_END);
    tamanho = ftell(fp) - pos_atual;

    //Volta a posicao original
    fseek(fp, pos_atual, SEEK_SET);

    return tamanho;
    
}

void print_registros(FILE *fp) {
    char buffer;
    fseek(fp, 0, SEEK_END);
    long int fimArquivo = ftell(fp);
    fseek(fp, 276, SEEK_SET);

    if(verificar_vazio(fp) <= 0) {
        printf("Registro inexistente.\n");
    } else {
        while(ftell(fp) != fimArquivo) {
            REG_DADOS *r = ler_regDados(fp);
            if (get_removido(r) == '0') {
                imprimir_registros(r);
            }
            free(r);
        } 
    }

    if(ferror(fp)) {
        printf("Falha no processamento do arquivo.\n");
    }
}