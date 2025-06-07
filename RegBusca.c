#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegPrint.h"
#include "RegBusca.h"

struct filtros {
	char parametro[20];
	char valor[20];
};



FILTROS *criarFiltro(char *parametro, char *valor) {
	FILTROS *filtro = (FILTROS*) malloc(sizeof(FILTROS) * 1);

	strcpy(filtro->parametro, parametro);
	strcpy(filtro->valor, valor);

	return filtro;	
}

/*
	Funcao que seleciona qual campo o usuario inseriou e faz a comparação de acordo com esse campo no registro
	Retorna 1 caso seja igual.
*/
int compararParametros(REG *r, FILTROS *filtro) {
	if(strcmp(filtro->parametro, "idAttack") == 0) {
		return atoi(filtro->valor) == get_idAttack(r);
	} else if(strcmp(filtro->parametro, "year") == 0) {
		return atoi(filtro->valor) == get_year(r);
	} else if(strcmp(filtro->parametro, "financialLoss") == 0) {
		return atof(filtro->valor) == get_financialLoss(r);
	} else if(strcmp(filtro->parametro, "country") == 0) {
		return strcmp(filtro->valor, get_country(r)) == 0;
	} else if(strcmp(filtro->parametro, "attacktype") == 0) {
		return strcmp(filtro->valor, get_attackType(r)) == 0;
	} else if(strcmp(filtro->parametro, "targetIndustry") == 0) {
		return strcmp(filtro->valor, get_targetIndustry(r)) == 0;
	} else if(strcmp(filtro->parametro, "defenseMechanism") == 0) {
		return strcmp(filtro->valor, get_defenseMechanism(r)) == 0;
	}

	return 0;
}

/*
	Funcao de busca sequencial de um registro de acordo com n filtros
	A funcao percorre o arquivo fp e le os registro um a um. 
	Compara-se o filtro com o valor do registro, caso todos sejam iguais,
	ela imprime o registro na tela.

	Essa busca reprete n vezes.
*/
void busca_registro(FILE *fp, FILTROS *filtros, int quantidadeFiltros, int quantidadeBuscas) {
	char buffer;
	int registrosEncontrados = 0;
	rewind(fp);
	fseek(fp, 0, SEEK_END);
    long int fimArquivo = ftell(fp);
    fseek(fp, 276, SEEK_SET); // Posiciona o ponteiro apos o cabecalho

    if(verificar_vazio(fp) == 0) { // Verifica se ha registros no arquivo
        printf("Registro inexistente.\n");
    } else {
		while(ftell(fp) != fimArquivo) { // Equanto o ponteiro fp não chega no fim do arquivo
			REG *r = ler_regDados(fp); // Le um registro do arquivo
			if(get_removido(r) == '0') {
				int auxiliar = 1;
				for(int i = 0; i < quantidadeFiltros; i++) { // Faz a iteração de acordo com a quantidade de filtros
					if(!compararParametros(r, &filtros[i])) {
						auxiliar = 0; // Variavel auxiliar, caso seja 0, nao imprime o registro
						free(r);
						break;
					}

				}
				if(auxiliar) { // Se os dois parametros forem iguais aos do registro
					imprimir_registros(r); // Imprime o registro
					registrosEncontrados++; // Aumenta o contador de registro encontrado
	
					if(registrosEncontrados > quantidadeBuscas) { // Verifica se ja encontramos o numero desejado pelo usuario
						free(r);
						break; // Sai do loop
					}
				}
			}

			free(r);
		}
		if(registrosEncontrados == 0) {
			printf("NADA CONSTA\n");
		}
    }

}

/*
	Funcao incabada
*/
void ler_linha_busca(FILE *fp) {
	/*int quantidadeBuscas = 0;
	char *nomeDoCampo = null;
	while(1) {
		scanf("%d", &quantidadeBuscas);
		scanf(" %s", nomeDoCampo);

	}*/
	FILTROS *filtros = criarFiltro("country", "BRAZIL");
	printf("FILTRO: %s\n", filtros->parametro);
	printf("VALOR: %s\n", filtros->valor);
	busca_registro(fp, filtros, 1, 1);
	free(filtros);
	
}