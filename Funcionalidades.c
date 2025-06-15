#include "RegBusca.h"
#include "RegPrint.h"
#include "RegRW.h"
#include "csvRead.h"
#include <stdlib.h>
#include <stdio.h>
#include "remocao.h"
#include "insertInto.h"
#include "update.h"
/*--------------------------------Funcção fornecida--------------------------*/
#include <ctype.h>
#include <string.h>
void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

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


/*---------------------------------------------------------------------------*/


void ler_campos_valores(int tamanho, char *campos[], char* valores[]){
	
		for(int i = 0;i<tamanho;i++){
			campos[i] = malloc(sizeof(char)*50);
			valores[i] = malloc(sizeof(char)*50);

			scanf(" %s",campos[i]);
			if(strcmp(campos[i],"idAttack") == 0 || strcmp(campos[i],"financialLoss") == 0 || strcmp(campos[i],"year") == 0){
				scanf("%s",valores[i]);
			}
			else{
				scan_quote_string(valores[i]);
			}

		}

}


/* função para executar a funcionalidade 1 */
bool func1(char* string1, char*string2){
    if (!CSV_to_BIN(string1,string2)){
        printf("Falha no processamento do arquivo.\n");
        return false;
    }
    binarioNaTela(string2);
    
    return true;
    

}
/*Funcção responsável para chamar as funções auxiliares que executam o que a funcionalidade 2 exige*/
bool func2(char * string1){
    printar_binario(string1);
    return true;
}

bool func3(char* nomeArquivo){
	int n;
	char nchar[20];
	int tamanho = 0;
	char tamchar[20];

	scanf("%s",nchar);
	n = atoi(nchar);
	
	for(int i = 0;i<n;i++){
		scanf("%s",tamchar);

		tamanho = atoi(tamchar);
		char* campos[tamanho];
		char * valores[tamanho];
		for(int i = 0;i<tamanho;i++){
			campos[i] = malloc(sizeof(char)*50);
			valores[i] = malloc(sizeof(char)*50);

			scanf(" %s",campos[i]);
			if(strcmp(campos[i],"idAttack") == 0 || strcmp(campos[i],"financialLoss") == 0 || strcmp(campos[i],"year") == 0){
				scanf("%s",valores[i]);
			}
			else{
				scan_quote_string(valores[i]);
			}

		}
	
		busca_registro(nomeArquivo, campos, valores, tamanho);
		
		for(int j = 0;j<tamanho;j++){
			free(campos[j]);
			free(valores[j]);
		}
		
		
	}
    return true;
}

void func4(char* nomebin){
	int n;
	char nchar[20];
	int tamanho;
	char tamchar[20];

	scanf("%s",nchar);
	n = atoi(nchar);
	
	for(int i = 0;i<n;i++){
		

		scanf("%s",tamchar);

		tamanho = atoi(tamchar);
		char* campos[tamanho];
		char * valores[tamanho];
		for(int i = 0;i<tamanho;i++){
			campos[i] = malloc(sizeof(char)*50);
			valores[i] = malloc(sizeof(char)*50);

			scanf(" %s",campos[i]);
			if(strcmp(campos[i],"idAttack") == 0 || strcmp(campos[i],"financialLoss") == 0 || strcmp(campos[i],"year") == 0){
				scanf("%s",valores[i]);
			}
			else{
				scan_quote_string(valores[i]);
			}

		}
		
		
		remover_registros(nomebin, campos,valores,tamanho);
		
		for(int j = 0;j<tamanho;j++){
			free(campos[j]);
			free(valores[j]);
		}
		
		
	}
	
	binarioNaTela(nomebin);

}

void func6(char* nomebin){
	int n = -1;
	char tamcharFiltro[50];
	int tamanhoFiltro;
	char tamcharCampos[50];
	int tamanhoCampos;
	scanf("%d",&n);

	for(int i = 0;i<n;i++){
	
		scanf("%s",tamcharFiltro);
		tamanhoFiltro = atoi(tamcharFiltro);

		char* camposF[tamanhoFiltro];
		char* valoresF[tamanhoFiltro];
		ler_campos_valores(tamanhoFiltro,camposF,valoresF);


		scanf("%s",tamcharCampos);
		tamanhoCampos = atoi(tamcharCampos);

		char* campos[tamanhoCampos];
		char * valores[tamanhoCampos];
		ler_campos_valores(tamanhoCampos,campos,valores);
		
		
		
		atualizar_reg(nomebin, tamanhoFiltro,camposF,valoresF,tamanhoCampos,campos,valores);

		for(int j = 0;j<tamanhoFiltro;j++){
			free(camposF[j]);
			free(valoresF[j]);
		}
		for(int j = 0;j<tamanhoCampos;j++){
			free(campos[j]);
			free(valores[j]);
		}
		

	}
	binarioNaTela(nomebin);

}

void func5(char* nomebin) {
	int numeroInsercoes;
	scanf("%d", &numeroInsercoes);

	insertInto(nomebin, numeroInsercoes);

	binarioNaTela(nomebin);
}

void ler_entradas(){
    int func;
    char string1[20];
    char string2[20];

    scanf("%d",&func);
    scanf("%s",string1);
    
	switch(func) {
		case 1:
			scanf("%s",string2);
			func1(string1,string2);
			break;
		case 2:
			func2(string1);
			break;
		case 3:
			func3(string1);
			break;
		case 4:
			func4(string1);
			break;
		case 5:
			func5(string1);
			break;
		case 6:
			func6(string1);
			break;
		default:
		break;
	}

    /*if(func == 1){
        scanf("%s",string2);
        func1(string1,string2);
    }
	if(func == 2){
        func2(string1);
    }
    if(func == 3){
        func3(string1);

    }
	if(func == 4){
		func4(string1);
	}

	if(func == 6){
		func6(string1);
	}
	if*/
    

    
}