#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RegRW.h"
#include "RegPrint.h"

void imprimir_registros(REG *r) {
    printf("IDENTIFICADOR DO ATAQUE: %d\n", get_idAttack(r));
    printf("ANO EM QUE O ATAQUE OCORREU: %d\n", get_year(r));
    printf("PAIS ONDE OCORREU O ATAQUE: %s\n", get_country(r));
    printf("SETOR DA INDUSTRIA QUE SOFREU O ATAQUE: %s\n", get_targetIndustry(r));
    printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: %s\n", get_attackType(r));
    printf("PREJUIZO CAUSADO PELO ATAQUE: %f\n", get_financialLoss(r));
    printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: %s$$\n", get_defenseMechanism(r));
}

/*
    Funcao que verifica se o ponteiro para o arquivo fp aponta para o final do arquivo.
    Para isso, ele subtrai o valor da posicao final pela atual e retorna este valor


/*
    Função de imprimir todos os registros do arquivo binario.
    Recebe um ponteiro FILE para o arquivo binario,
    pula o cabecario e imprime os registros
*/
/*void print_registros(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long int fimArquivo = ftell(fp); // Armazena a posicao do final do arquivo
    fseek(fp, 276, SEEK_SET); // Pula o cabecalho (276 bytes)

    if(verificar_vazio(fp) <= 0) { // Verifica se há registros nesse arquivo
        printf("Registro inexistente.\n");
    } else {
        while(ftell(fp) != fimArquivo) { // Verifica se chegou ao fim do arquivo
            REG *r = ler_regDados(fp); // Le, do arquivo binario, um registro e o armazena em r
            if (get_removido(r) == '0') {
                imprimir_registros(r); // Caso o registro nao for removido, imprimi-lo
            }
            free(r);
        } 
    }

    if(ferror(fp)) {
        printf("Falha no processamento do arquivo.\n");
    }
}*/