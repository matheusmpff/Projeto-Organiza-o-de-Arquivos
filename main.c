#include "RegBusca.h"
#include "RegPrint.h"
#include "RegRW.h"
#include "csvRead.h"
#include <stdlib.h>
#include <stdio.h>
#include "remocao.h"
#include "insertInto.h"
#include "update.h"
#include "arvoreB.h"
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

/*
Parâmetros:
	tamanho -> quantidade de pares campo-valor que serão lidos
	campos,valores -> arrays que irão armazenar os campos e os valores deles para utilização nas funcionalidades
*/
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


/* Parâmetros:
	string1 -> nome do arquivo CSV que deve ser lido
	string2 -< nome do arquivo binário que deve ser criado
A função cria um arquivo binário a partir dos dados do arquivo CSV. A função auxiliar CSV_to_BIN
será responsável por manipular os arquivos e criar o binário. Caso não seja possível criá-lo ele retorna falso
e é mostrado na tela a mensagem de erro, caso contrário executa a função fornecida binarioNaTela 	
*/
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
			if(strcmp(campos[i],"idAttack") == 0){
				scanf("%s",valores[i]);
			}
			else if(strcmp(campos[i],"year") == 0) {
				scanf("%s",valores[i]);
				if(strcmp(valores[i], "NULO") == 0){
					strcpy(valores[i], "-1");
				}
			}
			else if(strcmp(campos[i],"financialLoss") == 0) {
				scanf("%s",valores[i]);
				if(strcmp(valores[i], "NULO") == 0){
					strcpy(valores[i], "-1");
				}
			}
			else{
				scan_quote_string(valores[i]);
				if(strcmp(valores[i], "NULO") == 0){
					strcpy(valores[i], "-1000");
				}
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

long int* func5(char* nomebin) {
	int numeroInsercoes;
	scanf("%d", &numeroInsercoes);

	long int* valores = insertInto(nomebin, numeroInsercoes);

	binarioNaTela(nomebin);

	return valores;
}

void func7(char * nomebin){
	char nomeindice[20];
	scanf("%s",nomeindice);
	if(criar_indice(nomebin,nomeindice)){
		binarioNaTela(nomeindice);
	}
	else{
		printf("Erro na criação do indice\n");
	}
}

void func8(char * nomebin){
	char nomeindice[20];
	scanf("%s",nomeindice);

	int n;
	char nchar[20];
	int tamanho = 0;
	char tamchar[20];

	scanf("%s",nchar);
	n = atoi(nchar);
	
	int flagId = 0;
	for(int i = 0;i<n;i++){
		scanf("%s",tamchar);
		flagId = 0;
		tamanho = atoi(tamchar);
		char* campos[tamanho];
		char * valores[tamanho];
		for(int i = 0;i<tamanho;i++){
			campos[i] = malloc(sizeof(char)*50);
			valores[i] = malloc(sizeof(char)*50);

			scanf(" %s",campos[i]);
			if(strcmp(campos[i],"idAttack") == 0){
				scanf("%s",valores[i]);
				flagId = atoi(valores[i]);
			}
			else if(strcmp(campos[i],"year") == 0) {
				scanf("%s",valores[i]);
				if(strcmp(valores[i], "NULO") == 0){
					strcpy(valores[i], "-1");
				}
			}
			else if(strcmp(campos[i],"financialLoss") == 0) {
				scanf("%s",valores[i]);
				if(strcmp(valores[i], "NULO") == 0){
					strcpy(valores[i], "-1.0");
				}
			}
			else{
				scan_quote_string(valores[i]);
				if(strcmp(valores[i], "") == 0){
					strcpy(valores[i], "-1000");
				}
			}

		}
		if(flagId == 0){
			busca_registro(nomebin, campos, valores, tamanho);
		}
		else{
			busca_arvore(nomebin,nomeindice,campos,valores,tamanho,flagId);
		}
		for(int j = 0;j<tamanho;j++){
			free(campos[j]);
			free(valores[j]);
		}
	}
	

}

void func10(char* nomebin){
	char nomeindice[20];
	scanf("%s",nomeindice);
	long int* valores = func5(nomebin);

	int tamanho = 0;
	for(int i = 0;valores[i]!=-1;i++){
		tamanho++;
	}
	ajuste_indice_inserInto(nomebin,nomeindice,valores,tamanho);

	binarioNaTela(nomeindice);

	free(valores);
}

void func11(char* nomebin) {
	char nomeIndice[20];
	scanf("%s", nomeIndice);
	int numeroUpdate = 0;
	scanf("%d", &numeroUpdate);

	executar_update(nomebin, nomeIndice, numeroUpdate);

}


int main(){
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
		case 7:
			func7(string1);
			break;
		case 8:
			func8(string1);
			break;
		case 10:
			func10(string1);
		case 11:
			func11(string1);
		default:
		break;
	}
    
}