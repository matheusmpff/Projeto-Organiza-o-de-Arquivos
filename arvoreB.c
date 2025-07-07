
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "remocao.h"
#include <math.h>
#include "insertInto.h"
#include "main.h"
#include "RegBusca.h"

#define TamMax 1000

#define TamNo 44

typedef struct arvbheader{
    char status;
    int noRaiz;
    int proxRRN;
    int nroNos;
}ArvBHeader;

typedef struct arvBNo{
    int chaves[2];
    long int registros[2];
    int descendentes[3];
    int tipoNo;
    int nroChaves;
    
}NO;

typedef struct chave_valor{
    int chave;
    int valor;

    int esq;
    int dir;

}CHAVE_VALOR;

CHAVE_VALOR* criar_dados(){
    CHAVE_VALOR* dados = malloc(sizeof(CHAVE_VALOR));

    if(dados == NULL){
        printf("Problema na alocação\n");
        return dados;
    }

    dados->chave = -1;
    dados->valor = -1;
    dados->esq = -1;
    dados->dir = -1;

    return dados;
}

ArvBHeader* criar_arvB_header(){
    ArvBHeader* header = malloc(sizeof(ArvBHeader));

    if(header == NULL){
        printf("Falha na alocação de memória");
        return header;
    }

    header->status = '1';
    header->noRaiz = -1;
    header->proxRRN = 0;
    header->nroNos = 0;

    return header;
}



NO * criar_no(){
    NO* no = malloc(sizeof(NO));

    if(no == NULL){
        printf("Falha na alocação de memória");
        return no;
    }

    for(int i = 0;i<2;i++){
        no->chaves[i] = -1;
        no->registros[i] = -1;
        no->descendentes[i] = -1;
    }
    no->descendentes[2] = -1;

    no->tipoNo = -1;
    no->nroChaves = 0;

    return no;
}

void escrever_arvB_header(FILE* bin,ArvBHeader* header){
    fseek(bin,0,SEEK_SET);

    fwrite(&header->status,sizeof(char),1,bin);
    fwrite(&header->noRaiz,sizeof(int),1,bin);
    fwrite(&header->proxRRN,sizeof(int),1,bin);
    fwrite(&header->nroNos,sizeof(int),1,bin);
    fflush(bin);
    
    for(int i = 0;i<31;i++){
        fwrite("$",sizeof(char),1,bin);
    }
}

void ler_arvB_header(FILE *indice,ArvBHeader * header){
    fseek(indice,0,SEEK_SET);
    fread(&header->status,sizeof(char),1,indice);
    fread(&header->noRaiz,sizeof(int),1,indice);
    fread(&header->proxRRN,sizeof(int),1,indice);
    fread(&header->nroNos,sizeof(int),1,indice);
    fseek(indice,32,SEEK_CUR);
}
void ler_no_indice(FILE* indice,NO* no, int RRN){
    int tamanho = RRN*44+44;
    fseek(indice,tamanho,SEEK_SET);
    fread(&no->tipoNo,sizeof(int),1,indice);
    fread(&no->nroChaves,sizeof(int),1,indice);
    fread(&no->descendentes[0],sizeof(int),1,indice);
    fread(&no->chaves[0],sizeof(int),1,indice);
    fread(&no->registros[0],sizeof(long int),1,indice);
    fread(&no->descendentes[1],sizeof(int),1,indice);
    fread(&no->chaves[1],sizeof(int),1,indice);
    fread(&no->registros[1],sizeof(long int),1,indice);
    fread(&no->descendentes[2],sizeof(int),1,indice);

}

void escrever_no_indice(FILE* indice,NO* no,int RRN){
    int tamanho = RRN*44+44;
    fseek(indice,tamanho,SEEK_SET);
    // printf("tipo no %d\n",no->tipoNo);
    fwrite(&no->tipoNo,sizeof(int),1,indice);
    fwrite(&no->nroChaves,sizeof(int),1,indice);
    fwrite(&no->descendentes[0],sizeof(int),1,indice);
    fwrite(&no->chaves[0],sizeof(int),1,indice);
    fwrite(&no->registros[0],sizeof(long int),1,indice);
    fwrite(&no->descendentes[1],sizeof(int),1,indice);
    fwrite(&no->chaves[1],sizeof(int),1,indice);
    fwrite(&no->registros[1],sizeof(long int),1,indice);
    fwrite(&no->descendentes[2],sizeof(int),1,indice);
    fflush(indice);
}


CHAVE_VALOR* insercao(FILE* indice,CHAVE_VALOR* dados, NO* no,ArvBHeader* h, int RRN,int contador1,int contador2,int contador3);
void inserir_chave(CHAVE_VALOR* dados, NO* no);
CHAVE_VALOR* split(CHAVE_VALOR* dados, NO* noesq,int RRNesq, ArvBHeader* h, FILE* indice);
CHAVE_VALOR* aux_insercao_promocao(NO* no, ArvBHeader* h, FILE* indice, int RRN,CHAVE_VALOR* dados, int index,int c1, int c2, int c3);

bool criar_indice(char* nomeBin, char* nomeIndice){

    FILE* bin = fopen(nomeBin,"rb+");
    FILE * indice = fopen(nomeIndice,"wb");

    if(indice == NULL){
        printf("Falha no processamento do arquivo\n");
    }
    fclose(indice);

    indice = fopen(nomeIndice,"rb+");
    if(bin == NULL || indice == NULL){
        printf("Falha no processamento do arquivo.\n");
        return false;
    }

    char status;
    fread(&status,sizeof(char),1,bin);

    if(status == '0'){
        printf("Arquivo inconsistente\n");
        return false;
    }
    fseek(bin,0,SEEK_END);
    long int final = ftell(bin);
    fseek(bin,0,SEEK_SET);

    HEADER* h = criar_header();
    ler_header(bin,h);

    ArvBHeader* Bheader = criar_arvB_header();
    NO* raiz = criar_no();
    
    Bheader->noRaiz = 0;
    Bheader->nroNos++;
    Bheader->proxRRN++;

    escrever_no_indice(indice,raiz,Bheader->noRaiz);
    int contador1 = 0;
    int contador2 = 0;
    int contador3 = 0;
    int i = 0;
    while(ftell(bin)!=final){
        i++;
        REG* reg = ler_registro(bin,h);
        
        if(get_removido(reg) == '0'){
            CHAVE_VALOR* dados = criar_dados();
            dados->chave = get_idAttack(reg);
            dados->valor = ftell(bin) - get_tamanhoRegistro(reg) -5;
            // printf("Inserindo ID %d - raiz: %d, proxRRN: %d\n", dados->chave, Bheader->noRaiz, Bheader->proxRRN);
            ler_no_indice(indice,raiz,Bheader->noRaiz);
            // printf("Raiz: %d ----- %d\n",raiz->chaves[0],raiz->chaves[1]);
            // printf("%d\n",raiz->tipoNo);
            // printf("%d\n",Bheader->noRaiz);
            // printf("%d\n",Bheader->proxRRN);
            
            insercao(indice,dados,raiz,Bheader,Bheader->noRaiz, contador1,contador2,contador3);
            
        }
        free(reg);
    }
    escrever_arvB_header(indice,Bheader);
    free(raiz);
    free(h);
    free(Bheader);
    fclose(bin);
    fclose(indice);
    return true;
}

CHAVE_VALOR* insercao(FILE* indice,CHAVE_VALOR* dados, NO* no,ArvBHeader* h, int RRN, int contador1, int contador2, int contador3){
    if(no->tipoNo == -1 && no->nroChaves<2){
        inserir_chave(dados,no);
        escrever_no_indice(indice,no,RRN);
        return NULL;
    }

    if(no->tipoNo == -1 && no->nroChaves == 2){
        if(RRN == h->noRaiz){
            //Realiza o split e adiciona os ponteiros do item que está sendo promovido
            CHAVE_VALOR* promocao = split(dados,no,RRN,h,indice);
            
            // printf("oiee\n");
            //Cria o novo no raiz
            NO* novaRaiz = criar_no();
            novaRaiz->tipoNo = 0;
            inserir_chave(promocao,novaRaiz);
            
            // printf("aqui tem que ser >%d\n",novaRaiz->tipoNo);

            //Atualiza os valores do header para o no raiz e o proxRRN
            h->noRaiz = h->proxRRN;
            h->proxRRN++;
            h->nroNos++;
            escrever_no_indice(indice,novaRaiz,h->noRaiz);
            NO* oie = criar_no();

            ler_no_indice(indice,oie,h->noRaiz);
            // printf("leitura dps de escrever: %d\n",oie->tipoNo);
            // printf("sfaf     %d",oie->chaves[0]);
            
            

            return NULL;
        }
        return split(dados,no,RRN,h,indice);
    }
    
    if(no->tipoNo == 0){
        // printf("Eu entrei aqui\n");
         if(no->chaves[0]> dados->chave){
            // printf("Entrei aqui1");
            return aux_insercao_promocao(no,h,indice,RRN,dados,0,contador1,contador2,contador3);
            
        }
        else if(no->chaves[1]>dados->chave || no->chaves[1] == -1){
            // printf("Entrei aqui2");
            return aux_insercao_promocao(no,h,indice,RRN,dados,1,contador1,contador2,contador3);
            
        }
        else{
            // printf("Entrei aqui");
            return aux_insercao_promocao(no,h,indice,RRN,dados,2,contador1,contador2,contador3);
        }
    }

    if(no->tipoNo == 1){
        if(no->chaves[0]> dados->chave){
            return aux_insercao_promocao(no,h,indice,RRN,dados,0,contador1,contador2,contador3);
        }
        else if(no->chaves[1]>dados->chave || no->chaves[1] == -1){
            return aux_insercao_promocao(no,h,indice,RRN,dados,1,contador1,contador2,contador3);
        }
        else{
            return aux_insercao_promocao(no,h,indice,RRN,dados,2,contador1,contador2,contador3);
        }

    }
    
    return NULL;
}




void inserir_chave(CHAVE_VALOR* dados, NO* no){
    if(no->nroChaves == 0){
        no->chaves[0] = dados->chave;
        no->registros[0] = dados->valor;
        no->descendentes[0] = dados->esq;
        no->descendentes[1] = dados->dir;
    }
    else if(no->chaves[0]> dados->chave){
        no->chaves[1] = no->chaves[0];
        no->registros[1] = no->registros[0];
        //ajuste dos descendentes
        no->descendentes[2] = no->descendentes[1];
        no->chaves[0] = dados->chave;
        no->registros[0] = dados->valor;
        no->descendentes[0] = dados->esq;
        no->descendentes[1] = dados->dir;

    }
    else{
        no->chaves[1] = dados->chave;
        no->registros[1] = dados->valor;
        no->descendentes[1] = dados->esq;
        no->descendentes[2] = dados->dir;
    }
    no->nroChaves++;
}
CHAVE_VALOR* split(CHAVE_VALOR* dados, NO* noesq, int RRNesq, ArvBHeader* h, FILE* indice){
    //Cria o nó da direita e a struct que passa os dados da promoção
    NO* nodir = criar_no();
    CHAVE_VALOR* promocao = criar_dados();
    if(dados->chave < noesq->chaves[0]){
        //criação do no da direita
        nodir->tipoNo = noesq->tipoNo;
        nodir->nroChaves++;
        nodir->chaves[0] = noesq->chaves[1];
        nodir->registros[0] = noesq->registros[1];
        nodir->descendentes[0] = noesq->descendentes[1];
        nodir->descendentes[1] = noesq->descendentes[2];
        //dados que serão retornados pela recursão para fazer a promoção
        promocao->chave = noesq->chaves[0];
        promocao->valor = noesq->registros[0];
        //Ajuste do nó da esquerda
        noesq->chaves[0] = dados->chave;
        noesq->registros[0] = dados->valor;
        noesq->descendentes[0] = dados->esq;
        noesq->descendentes[1] = dados->dir;
        
        noesq->chaves[1] = -1;
        noesq->registros[1] = -1;
        noesq->descendentes[2] = -1;
        noesq->nroChaves--;

        //Escrever os nós novamente no arquivo de dados
        escrever_no_indice(indice,noesq,RRNesq);
        escrever_no_indice(indice,nodir,h->proxRRN);

        promocao->esq = RRNesq;
        promocao->dir = h->proxRRN;
        h->proxRRN++;
        h->nroNos++;
        free(nodir);
        return promocao;
    }

    if(dados->chave > noesq->chaves[0] && dados->chave < noesq->chaves[1]){

         //criação do no da direita
        nodir->nroChaves++;
        nodir->tipoNo = noesq->tipoNo;
        nodir->chaves[0] = noesq->chaves[1];
        nodir->registros[0] = noesq->registros[1];
        nodir->descendentes[0] = dados->dir;
        nodir->descendentes[1] = noesq->descendentes[2];
        //dados que serão retornados pela recursão para fazer a promoção
        promocao->chave = dados->chave;
        promocao->valor = dados->valor;
        //Ajuste do nó da esquerda
        noesq->chaves[1] = -1;
        noesq->registros[1] = -1;
        noesq->descendentes[2] = -1;
        noesq->nroChaves--;

        //Escrever os nós novamente no arquivo de dados
        escrever_no_indice(indice,noesq,RRNesq);
        escrever_no_indice(indice,nodir,h->proxRRN);

        promocao->esq = RRNesq;
        promocao->dir = h->proxRRN;
        h->proxRRN++;
        h->nroNos++;
        free(nodir);
        return promocao;
    }

    if(dados->chave > noesq->chaves[1]){
        //criação do no da direita
        nodir->nroChaves++;
        nodir->tipoNo = noesq->tipoNo;
        nodir->chaves[0] = dados->chave;
        nodir->registros[0] = dados->valor;
        nodir->descendentes[0] = dados->esq;
        nodir->descendentes[1] = dados->dir;
        //dados que serão retornados pela recursão para fazer a promoção
        promocao->chave = noesq->chaves[1];
        promocao->valor = noesq->registros[1];
        //Ajuste do nó da esquerda
        noesq->chaves[1] = -1;
        noesq->registros[1] = -1;
        noesq->descendentes[2] = -1;
        noesq->nroChaves--;

        //Escrever os nós novamente no arquivo de dados
        escrever_no_indice(indice,noesq,RRNesq);
        escrever_no_indice(indice,nodir,h->proxRRN);

        promocao->esq = RRNesq;
        promocao->dir = h->proxRRN;

        h->proxRRN++;
        h->nroNos++;
        free(nodir);
        return promocao;

    }
    free(nodir);
    return NULL;

}

CHAVE_VALOR* aux_insercao_promocao(NO* no, ArvBHeader* h, FILE* indice, int RRN,CHAVE_VALOR* dados, int index, int c1, int c2, int c3){
    NO* descendente = criar_no();
    ler_no_indice(indice,descendente,no->descendentes[index]);
    CHAVE_VALOR* promocao = insercao(indice,dados,descendente,h,no->descendentes[index],c1,c2,c3);
    if(promocao == NULL){
        free(descendente);
        return NULL;
    }
    //ajuste dos descendentes para fazer o split adequados
    if(no->nroChaves<2){
    //Caso em que nó intermediário tem espaço para promoção
        inserir_chave(promocao,no);
        escrever_no_indice(indice,no,RRN);
        free(descendente);
        free(promocao);
        return NULL;         
    }
    else{
        //Caso em que está cheio
        if(RRN != h->noRaiz ){
            //Caso em que o nó não é o raiz
            CHAVE_VALOR* novapromocao = split(promocao,no,RRN,h,indice);
            free(descendente);
            return(novapromocao);
        }   
        else{
        //Caso para quando é o nó raiz que recebe a promoção
            if(no->tipoNo == 0){
                no->tipoNo = 1;
            }
            CHAVE_VALOR* promocaoRaiz = split(promocao,no,RRN,h,indice);
            //Criação do novo nó raiz
            NO* novaRaiz = criar_no();
            novaRaiz->tipoNo = 0;
            inserir_chave(promocaoRaiz,novaRaiz);
            //escreve o novo no arquivo
            escrever_no_indice(indice,novaRaiz,h->proxRRN);
            //ajuste dos valores do header
            h->noRaiz = h->proxRRN;
            h->proxRRN++;
            h->nroNos++;

            free(descendente);
            free(promocao);
            free(promocaoRaiz);
            return NULL;
        }
    }


    return NULL;
}

/*-------------------------------------------------SELECT ----------------------------------------------------------------------*/
long int busca_id_arvB(FILE* indice,int id,NO* no);
void busca_arvore(char* nomebin, char* nomeindice,char* campos[],char*valores[],int tamanho,int id){
    FILE* bin = fopen(nomebin,"rb+");
    FILE* indice = fopen(nomeindice,"rb+");

    if(bin == NULL || indice == NULL ){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    ArvBHeader* h = criar_arvB_header();
    ler_arvB_header(indice,h);
    NO* raiz = criar_no();
    ler_no_indice(indice,raiz,h->noRaiz);
    long int posicaoReg = busca_id_arvB(indice,id,raiz);

    HEADER* headerReg = criar_header();
    ler_header(bin,headerReg);
    if(posicaoReg != -1){
        fseek(bin,posicaoReg,SEEK_SET);
        REG* reg = ler_registro(bin,headerReg);
        printar_registro(reg,headerReg);
        free(reg);
    }
    else{
		printf("Registro inexistente.\n\n");
	}
	printf("**********\n");
    fclose(bin);
    fclose(indice);
    free(headerReg);
    free(raiz);
    free(h);
}

long int busca_id_arvB(FILE* indice,int id,NO* no){
    if(no->tipoNo == -1 && no->chaves[0] != id && no->chaves[1] != id){
        return -1;
    }

    if(no->chaves[0] == id){
        return no->registros[0];
    }

    if(no->chaves[1] == id){
        return no->registros[1];
    }

    NO* descendente = criar_no();
    long int resultado = -1;
    if(no->descendentes[0] != -1 && id<no->chaves[0]){

        ler_no_indice(indice,descendente,no->descendentes[0]);
        resultado = busca_id_arvB(indice,id,descendente);
        free(descendente);
        return(resultado);
    }
    if(no->descendentes[1] != -1 && id>no->chaves[0] && (id<no->chaves[1] || no->chaves[1] == -1)){
        ler_no_indice(indice,descendente,no->descendentes[1]);
        resultado = busca_id_arvB(indice,id,descendente);
        free(descendente);
        return(resultado);
    }
    if(no->descendentes[2] != -1 && id> no->chaves[1]){
        ler_no_indice(indice,descendente,no->descendentes[2]);
        resultado = busca_id_arvB(indice,id,descendente);
        free(descendente);
        return(resultado);
    }
    
    free(descendente);
    return -1;

}

void ajuste_indice_inserInto(char* nomebin,char* nomeindice,long int *valores,int tamanho){
    FILE* bin = fopen(nomebin,"rb+");
    FILE* indice = fopen(nomeindice,"rb+");

    if(bin == NULL || indice == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    HEADER* binHeader = criar_header();
    ler_header(bin, binHeader);

    ArvBHeader * indiceHeader = criar_arvB_header();
    ler_arvB_header(indice,indiceHeader);
    

    NO* raiz = criar_no();

    for(int i = 0;i<tamanho;i++){

        fseek(bin,valores[i],SEEK_SET);
        REG* reg = ler_registro(bin,binHeader);
        CHAVE_VALOR * novaChave = criar_dados();
        novaChave->chave = get_idAttack(reg);
        novaChave->valor = valores[i];

        ler_no_indice(indice,raiz,indiceHeader->noRaiz);
        insercao(indice,novaChave,raiz,indiceHeader,indiceHeader->noRaiz,0,0,0);
        free(reg);
    }

    escrever_arvB_header(indice,indiceHeader);
    free(raiz);
    free(indiceHeader);
    free(binHeader);
    fclose(bin);
    fclose(indice);
}

/* Funcionalidade 11 */

void buscar_registros_para_update(FILE* bin, HEADER* header, char* campos[], char* valores[], int numCampos, int** ids_encontrados, int* count) {
    fseek(bin, 0, SEEK_END);
    long int final = ftell(bin);
    fseek(bin, 0, SEEK_SET);

    HEADER* buffer = criar_header();
    ler_header(bin, buffer);
    free(buffer);
    //fseek(bin, 44, SEEK_SET); // Pula o header
    
    *count = 0;
    int capacidade = 100;
    *ids_encontrados = malloc(capacidade * sizeof(int));
    
    while(ftell(bin) < final) {
        REG* reg = ler_registro(bin, header);
        
        if(get_removido(reg) == '0') { // Registro não removido
            bool atende_criterios = true;

            atende_criterios = comparCampos(reg, campos, valores, numCampos);
            
            if(atende_criterios) {
                if(*count >= capacidade) {
                    capacidade *= 2;
                    *ids_encontrados = realloc(*ids_encontrados, capacidade * sizeof(int));
                }
                (*ids_encontrados)[*count] = get_idAttack(reg);
                (*count)++;
            }
        }
        
        free(reg);
    }
}

bool atualizar_registro(FILE* bin, FILE* indice, HEADER* header, ArvBHeader* arvb_header, int id, char* campos_update[], char* valores_update[], int num_updates) {
    
    // Busca a posição do registro usando a árvore-B
    NO* raiz = criar_no();
    ler_no_indice(indice, raiz, arvb_header->noRaiz);
    long int posicao = busca_id_arvB(indice, id, raiz);
    free(raiz);
    
    if(posicao == -1) {
        return false; // Registro não encontrado
    }
    
    // Lê o registro atual
    fseek(bin, posicao, SEEK_SET);
    REG* reg_atual = ler_registro(bin, header);
    
    if(get_removido(reg_atual) == '1') {
        free(reg_atual);
        return false; // Registro já removido
    }
    
    // Cria uma cópia do registro para atualização
    REG* reg_atualizado = criar_reg();
    
    // Copia os dados atuais
    set_removido(reg_atualizado, '0');
    set_tamanhoRegistro(reg_atualizado, get_tamanhoRegistro(reg_atual));
    set_prox(reg_atualizado, get_prox(reg_atual));
    set_idAttack(reg_atualizado, get_idAttack(reg_atual));
    set_year(reg_atualizado, get_year(reg_atual));
    set_financialLoss(reg_atualizado, get_financialLoss(reg_atual));
    set_country(reg_atualizado, get_country(reg_atual));
    set_attackType(reg_atualizado, get_attackType(reg_atual));
    set_targetIndustry(reg_atualizado, get_targetIndustry(reg_atual));
    set_defenseMechanism(reg_atualizado, get_defenseMechanism(reg_atual));
    
    // Aplica as atualizações
    for(int i = 0; i < num_updates; i++) {
        if(strcmp(campos_update[i], "idAttack") == 0) {
            set_idAttack(reg_atualizado, atoi(valores_update[i]));
        } else if(strcmp(campos_update[i], "country") == 0) {
            set_country(reg_atualizado, valores_update[i]);
        } else if(strcmp(campos_update[i], "targetIndustry") == 0) {
            set_targetIndustry(reg_atualizado, valores_update[i]);
        } else if(strcmp(campos_update[i], "financialLoss") == 0) {
            set_financialLoss(reg_atualizado, atof(valores_update[i]));
        } else if(strcmp(campos_update[i], "attackType") == 0) {
            set_attackType(reg_atualizado, valores_update[i]);
        } else if(strcmp(campos_update[i], "defenseMechanism") == 0) {
            set_defenseMechanism(reg_atualizado, valores_update[i]);
        } else if(strcmp(campos_update[i], "year") == 0) {
            set_year(reg_atualizado, atoi(valores_update[i]));
        }
        // Adicionar outros campos conforme necessário
    }
    
    // Calcula o tamanho do registro atualizado
    int tamanho_atual = get_tamanhoRegistro(reg_atual);
    int tamanho_atualizado = calcularTamanhoRegistro(reg_atualizado);
    
    if(tamanho_atualizado <= tamanho_atual) {
        // Atualização in-place
        fseek(bin, posicao, SEEK_SET);
        escrever_registro(bin, reg_atualizado, header);
        
        // Preenche o resto com lixo '$'
        int espacoRestante = tamanho_atual + sizeof(char) /*char removido*/ + sizeof(int) /*int tamanho registro*/ - tamanho_atualizado;
        if(espacoRestante > 0) {
            char lixo = '$';
            for(long int j = 0; j < espacoRestante; j++) {
                fwrite(&lixo, sizeof(char), 1, bin);
            }
        }
        fflush(bin);
        
    } else {
        // AQUI TA ERRADO
        
        // Marca o registro atual como removido
        remover_registro(bin, reg_atual, header, posicao);
        
        // Insere o novo registro
        long int nova_posicao = inserir_registro_final(bin, header, reg_atualizado);
        
        // Atualiza o índice se o idAttack mudou
        if(get_idAttack(reg_atual) != get_idAttack(reg_atualizado)) {
            // Remove a entrada antiga do índice
            // Adiciona a nova entrada no índice
            CHAVE_VALOR* nova_chave = criar_dados();
            nova_chave->chave = get_idAttack(reg_atualizado);
            nova_chave->valor = nova_posicao;
            
            NO* raiz = criar_no();
            ler_no_indice(indice, raiz, arvb_header->noRaiz);
            insercao(indice, nova_chave, raiz, arvb_header, arvb_header->noRaiz, 0, 0, 0);
            free(raiz);
        }
        
        // Atualiza o header do arquivo de dados
        fseek(bin, 0, SEEK_SET);
        escrever_header(bin, header);
    }
    
    free(reg_atual);
    free(reg_atualizado);
    return true;
}
void executar_update(char* nomeBin, char* nomeIndice, int nUpdates) {
    FILE* bin = fopen(nomeBin, "rb+");
    FILE* indice = fopen(nomeIndice, "rb+");
    
    if(bin == NULL || indice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // Verifica consistência dos arquivos
    char statusBin, statusIndice;
    fread(&statusBin, sizeof(char), 1, bin);
    fread(&statusIndice, sizeof(char), 1, indice);
    
    if(statusBin == '0' || statusIndice == '0') {
        printf("Arquivo inconsistente.\n");
        fclose(bin);
        fclose(indice);
        return;
    }
    
    // Marca os arquivos como inconsistentes durante o processo
    fseek(bin, 0, SEEK_SET);
    fwrite("0", sizeof(char), 1, bin);
    fseek(indice, 0, SEEK_SET);
    fwrite("0", sizeof(char), 1, indice);

    fseek(bin, 0, SEEK_SET);
    fseek(indice, 0, SEEK_SET);
    
    // Lê os headers
    HEADER* headerDados = criar_header();
    ArvBHeader* headerIndice = criar_arvB_header();
    
    ler_header(bin, headerDados);
    ler_arvB_header(indice, headerIndice);
    
    // Processa cada update
    for(int update = 0; update < nUpdates; update++) {
        int m = 0;
        int p = 0;
        scanf("%d", &m);
    
        // Lê critérios de busca
        char* campos_busca[m];
        char* valores_busca[m];
        
        for(int i = 0; i < m; i++) {
            campos_busca[i] = malloc(50 * sizeof(char));
            valores_busca[i] = malloc(50 * sizeof(char));
            scanf("%s", campos_busca[i]);
            
            if(strcmp(campos_busca[i],"idAttack") == 0){
                scanf("%s",valores_busca[i]);
            }
            else if(strcmp(campos_busca[i],"year") == 0) {
                scanf("%s",valores_busca[i]);
                if(strcmp(valores_busca[i], "NULO") == 0){
                    strcpy(valores_busca[i], "-1");
                }
            }
            else if(strcmp(campos_busca[i],"financialLoss") == 0) {
                scanf("%s",valores_busca[i]);
                if(strcmp(valores_busca[i], "NULO") == 0){
                    strcpy(valores_busca[i], "-1.0");
                }
            }
            else{
                scan_quote_string(valores_busca[i]);
                if(strcmp(valores_busca[i], "") == 0){
                    strcpy(valores_busca[i], "-1000");
                }
            }
        }
        
        scanf("%d", &p);
        
        // Lê campos e valores para atualização
        char* campos_update[p];
        char* valores_update[p];
        
        for(int i = 0; i < p; i++) {
            campos_update[i] = malloc(50 * sizeof(char));
            valores_update[i] = malloc(50 * sizeof(char));
            scanf("%s", campos_update[i]);
            
            if(strcmp(campos_update[i],"idAttack") == 0){
                scanf("%s",valores_update[i]);
            }
            else if(strcmp(campos_update[i],"year") == 0) {
                scanf("%s",valores_update[i]);
                if(strcmp(valores_update[i], "NULO") == 0){
                    strcpy(valores_update[i], "-1");
                }
            }
            else if(strcmp(campos_update[i],"financialLoss") == 0) {
                scanf("%s",valores_update[i]);
                if(strcmp(valores_update[i], "NULO") == 0){
                    strcpy(valores_update[i], "-1.0");
                }
            }
            else{
                scan_quote_string(valores_update[i]);
                if(strcmp(valores_update[i], "") == 0){
                    strcpy(valores_update[i], "-1000");
                }
            }
        }

        
        // Busca registros que atendem aos critérios
        int* ids_encontrados;
        int count;
        buscar_registros_para_update(bin, headerDados, campos_busca, valores_busca, m, &ids_encontrados, &count);
        
        // Atualiza cada registro encontrado
        for(int i = 0; i < count; i++) {
            atualizar_registro(bin, indice, headerDados, headerIndice, ids_encontrados[i], campos_update, valores_update, p);
        }
        
        // Libera memória
        for(int i = 0; i < m; i++) {
            free(campos_busca[i]);
            free(valores_busca[i]);
        }

        for(int i = 0; i < p; i++) {
            free(campos_update[i]);
            free(valores_update[i]);
        }

        if(ids_encontrados) free(ids_encontrados);
    }
    
    // Atualiza os headers nos arquivos
    fseek(bin, 0, SEEK_SET);
    escrever_header(bin, headerDados);
    fseek(indice, 0, SEEK_SET);
    escrever_arvB_header(indice, headerIndice);
    
    // Marca os arquivos como consistentes
    fseek(bin, 0, SEEK_SET);
    fwrite("1", sizeof(char), 1, bin);
    fseek(indice, 0, SEEK_SET);
    fwrite("1", sizeof(char), 1, indice);
    fflush(bin);
    fflush(indice);
    
    // Chama binarioNaTela para mostrar os arquivos
    binarioNaTela(nomeBin);
    binarioNaTela(nomeIndice);
    
    // Libera memória e fecha arquivos
    free(headerDados);
    free(headerIndice);
    fclose(bin);
    fclose(indice);
}