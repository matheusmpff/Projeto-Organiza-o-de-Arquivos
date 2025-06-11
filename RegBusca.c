#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegBusca.h"
#include "remocao.h"

/*
    Funcao que verifica se o ponteiro para o arquivo fp aponta para o final do arquivo.
    Para isso, ele subtrai o valor da posicao final pela atual e retorna este valor
*/
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

/*
	Funcao de busca sequencial de um registro de acordo com n filtros
	A funcao percorre o arquivo fp e le os registro um a um. 
	Compara-se o filtro com o valor do registro, caso todos sejam iguais,
	ela imprime o registro na tela.

	Essa busca reprete n vezes.
*/
void busca_registro(char* nomeArquivo, char* campos[], char* valores[], int quantidadeCampos, int quantidadeBuscas) {
	FILE* fp = fopen(nomeArquivo, "rb");
	if (fp == NULL) {
		printf("Erro no processamento de arquivo\n");
		return;
	}

	int registrosEncontrados = 0;
	long int fimArquivo = fseek(fp, 0, SEEK_END);
	fseek(fp, 0, SEEK_SET);
	HEADER * h = criar_header();
	ler_header(fp,h);

	while(ftell(fp) != fimArquivo) { // Equanto o ponteiro fp não chega no fim do arquivo
		REG *r = ler_registro(fp, h); // Le um registro do arquivo
		if(get_removido(r) == '0') {
			if(compara_campos(r, campos, valores, quantidadeCampos)) {
				printar_registro(r, h); // Imprime o registro
				registrosEncontrados++; // Aumenta o contador de registro encontrado

				if(registrosEncontrados >= quantidadeBuscas) { // Verifica se ja encontramos o numero desejado pelo usuario
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
	free(h);

	fclose(fp);
}

/*
	Funcao incabada
*/
//void ler_linha_busca(FILE *fp) {
	/*int quantidadeBuscas = 0;
	char *nomeDoCampo = null;
	while(1) {
		scanf("%d", &quantidadeBuscas);
		scanf(" %s", nomeDoCampo);

	}*/
	/*FILTROS *filtros = criarFiltro("country", "BRAZIL");
	printf("FILTRO: %s\n", filtros->parametro);
	printf("VALOR: %s\n", filtros->valor);
	busca_registro(fp, filtros, 1, 1);
	free(filtros);
	
}*/
