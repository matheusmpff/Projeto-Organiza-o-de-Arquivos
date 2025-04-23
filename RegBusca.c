#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "RegRW.h"
#include "RegPrint.h"
#include "RegBusca.h"

struct filtros {
	char parametro[20];
	char valor[20];
};

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


/* ---------------- EXTRA ----------------

OPCIONAL: dicas sobre scanf() e fscanf():

scanf("%[^,]", string) -> lê até encontrar o caractere ',', não incluindo o mesmo na leitura.

Exemplo de entrada: "Oi, esse é um exemplo."
Nesse caso, o scanf("%[^,]") tem como resultado a string "Oi";

scanf("%[^\"]", string) -> lê até encontrar o caractere '"', não incluindo o mesmo na leitura.
scanf("%[^\n]", string) -> lê até encontrar o fim da linha, não incluindo o '\n' na leitura.

scanf("%*c") --> lê um char e não guarda em nenhuma variável, como se tivesse ignorado ele

*/

FILTROS *criarFiltro(char *parametro, char *valor) {
	FILTROS *filtro = (FILTROS*) malloc(sizeof(FILTROS) * 1);

	strcpy(filtro->parametro, parametro);
	strcpy(filtro->valor, valor);

	return filtro;	
}





int compararParametros(REG_DADOS *r, FILTROS *filtro) {
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

void busca_registrador(FILE *fp, FILTROS **filtros, int quantidadeFiltros, int quantidadeBuscas) {
	char buffer;
	int registrosEncontrados = 0;

    if(verificar_vazio == 0) {
        printf("Registro inexistente.\n");
    } else {
		while(fread(&buffer, sizeof(char), 1, fp) == 1) {
			fseek(fp, -1, SEEK_CUR);
			REG_DADOS *r = ler_regDados(fp);
			if(get_removido(r) == '0') {
				int auxiliar = 1;
				for(int i = 0; i < quantidadeFiltros; i++) {
					if(!compararParametros(r, &filtros[i])) {
						auxiliar = 0;
						break;
					}

				}
				if(auxiliar) {
					printf("achou busca\n");
					imprimir_registros(r);
					registrosEncontrados++;
	
					if(registrosEncontrados >= quantidadeBuscas)
						break;
				}
			}
		}
		if(registrosEncontrados == 0) {
			printf("NADA CONSTA\n");
		}
    }
}