#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegBusca.h"
#include "remocao.h"

bool comparCampos(REG* reg, char* campos[], char* valores[], int tamanho){
    for(int i = 0 ;i < tamanho; i++){
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
            if(atoi(valores[i]) != get_financialLoss(reg)){
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
        if(strcmp(campos[i],"") == 0){}
        if(strcmp(campos[i],"idAttack") == 0){
            if(atoi(valores[i]) != get_idAttack(reg)){
                return false;
            }
        }
        continue;
    }

    return true;
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
			if(comparCampos(r, campos, valores, quantidadeCampos)) {
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
