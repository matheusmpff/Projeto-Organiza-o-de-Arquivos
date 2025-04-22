#include <stdio.h>
#include <string.h>
#include <ctype.h>
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

REG_DADOS busca_registrador(FILE *fp, char *nomeDoCampo, void *valorDoCampo, int quantidadeBuscas) {
    REG_DADOS reg = criar_regDados();

    if(verificar_vazio == 0) {
        printf("Registro inexistente.\n");
    } else {
        for(int i = 0; i < quantidadeBuscas; i++) {
            while(fread(&buffer, sizeof(char), 1, fp) == 1) {
                fseek(fp, -1, SEEK_CUR);
                REG_DADOS r = ler_regDados(fp);
                if (r.removido == '0') {
                    comparar_registrador(r, nomeDoCampo, valorDoCampo);
                }
            } 
        }
    }



    return reg;
}