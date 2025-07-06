#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "RegRW.h"
#include "RegBusca.h"
#include "remocao.h"

/*
	Funcao que realiza a comparacao dos filtros desejados pelo usuario com um registro

	Caso o valor do campo de um filtro e o valor do parametro do registro for igual, retorna 1.a64l
	Caso contrario, 0.

	@param reg Registro a ser comparado
	@param campos Matriz de campos para filtro da busca
	@param valores Matriz de valores dos campos da busca
	@param tamanho Numero de pares de filtros a serem buscados
*/
bool comparCampos(REG* reg, char* campos[], char* valores[], int tamanho){
    for(int i = 0 ;i < tamanho + 1; i++){
        if(strcmp(campos[i],"country") == 0){ 
            if(strcmp(valores[i], get_country(reg)) != 0){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"attackType") == 0){
            if(strcmp(valores[i], get_attackType(reg)) != 0){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"financialLoss") == 0){
			float financialLoss = atof(valores[i]);
			float arredondado = roundf(get_financialLoss(reg) *100.0f) / 100.0f;
            if(financialLoss != arredondado){
                return false;
                
            }
            continue;
        }
        if(strcmp(campos[i],"defenseMechanism") == 0){
            if(strcmp(valores[i], get_defenseMechanism(reg)) != 0){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"targetIndustry") == 0){
            if(strcmp(valores[i], get_targetIndustry(reg)) != 0){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"year") == 0){
			if(atoi(valores[i]) != get_year(reg)) {
				return false;
			}
			continue;
		}
        if(strcmp(campos[i],"idAttack") == 0){
            if(atoi(valores[i]) != get_idAttack(reg)){
                return false;
            }
			continue;
        }
    }

    return true;
}

/*
	Funcao de busca sequencial de um registro de acordo com n filtros
	A funcao percorre o arquivo fp e le os registro um a um. 
	Compara-se o filtro com o valor do registro, caso todos sejam iguais,
	ela imprime o registro na tela.

	TRATAMENTO:
	- caso a busca nao encontre registros no arquivo, retorna "Registro inexistente."

	@param nomeArquivo Nome do arquivo binario
	@param campos Matriz de campos para filtro da busca
	@param valores Matriz de valores dos campos da busca
	@param quantidade Numero de pares de filtros a serem buscados
*/
void busca_registro(char* nomeArquivo, char* campos[], char* valores[], int quantidade) {
	FILE* fp = fopen(nomeArquivo, "rb");
	if (fp == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	int registrosEncontrados = 0; // variavel auxiliar para contagem de resultados da busca
	fseek(fp, 0, SEEK_END);
	long int fimArquivo = ftell(fp); // variavel para armazenar a posicao do final do arquivo
	fseek(fp, 0, SEEK_SET);
	HEADER * h = criar_header();
	ler_header(fp,h);

	// Loop de buscas
	while(ftell(fp) < fimArquivo) { // Equanto o ponteiro fp não chega no fim do arquivo
		REG *r = ler_registro(fp, h); // Le um registro do arquivo
		if(compara_campos(r, campos, valores, quantidade)) {
			if(get_removido(r)) {
				printar_registro(r, h); // Imprime o registro
				registrosEncontrados++; // Aumenta o contador de registro encontrado

				 
			}
		}

		free(r);
	}

	// Caso a busca nao retorne nenhum registro
	if(registrosEncontrados == 0) {
		printf("Registro inexistente.\n\n");
	}

	printf("**********\n");

	free(h);

	fclose(fp);
}
