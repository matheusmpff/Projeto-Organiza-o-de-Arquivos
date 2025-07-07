
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

/*Struct pra representar o header ddo arquivo da árvore b*/
typedef struct arvbheader{
    char status;
    int noRaiz;
    int proxRRN;
    int nroNos;
}ArvBHeader;

/*Struct para representar o nó da árvore b
    chaves-> serão os ids lidos do arquivo de dados
    registros-> serão os byteoffsets desses arquivos 
    tipoNo-> indica se o nó é raiz(0) , intermediário(1) ou folha(-1)
*/
typedef struct arvBNo{
    int chaves[2];
    long int registros[2];
    int descendentes[3];
    int tipoNo;
    int nroChaves;
    
}NO;

/*Struct para auxiliar na passagem dos dados quando a uma promoção de chave após um split do nó folha*/
typedef struct chave_valor{
    int chave;
    int valor;

    int esq;
    int dir;

}CHAVE_VALOR;

/*
Função que inicializa os campos da struct CHAVE_VALOR. Todos os campos são inicializados com -1
*/
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

/*
    Função para incialiar o campos da Struct header do arquivo de índice. Os campos status(1),noRaiz(-1),proxRRN(0) e nroNOs(0)
    são inicializados com os valores entre parênteses
 */
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


/*
    Função para criar um nó e inicializar os campos. Todos os campos são inciializados com -1 menos o nroChaves que é inicia
    lizado om 0
*/
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
/*Parâmetros:
    @params bin-> ponteiro para o arquivo de indice
    @params header -> ponteiro pra struct header que armazenas os dados do header a ser escrito
    A função desloca o ponteiro para o inicio do arquivo e escreve os valores da struct no arquivo de indice
*/
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
/*Parâmetros:
    @params indice -> ponteiro para o arquivo de índice
    @params header -> ponteiro para struct que irá armazenar os dados do header
    A função lê os dados do header no início do arquivo e armazena os dados dentro da struct header passada como parâmetro
    Ao final pula-se a região do header que tem lixo e já deixa o ponteiro no inicio da parte dos dados da árvore
*/
void ler_arvB_header(FILE *indice,ArvBHeader * header){
    fseek(indice,0,SEEK_SET);
    fread(&header->status,sizeof(char),1,indice);
    fread(&header->noRaiz,sizeof(int),1,indice);
    fread(&header->proxRRN,sizeof(int),1,indice);
    fread(&header->nroNos,sizeof(int),1,indice);
    fseek(indice,32,SEEK_CUR);
}
/*Parâmetros:
    @params indice -> ponteiro para o arquivo de indice
    @params no -> ponteiro para struct NO que irá conter os dados do nó lidos no arquivo de índice
    @params RRN -> RRN do No que deseja-se ler
    A função faz o deslocamento do ponteiro para a região do arquivo onde está o nó, lê os dados e armazena na struct NO
*/
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
/*Parâmetros:
    @params indice -> ponteiro para o arquivo de indice
    @params no -> ponteiro para a struct que guarda os dados a serem escritos
    @params RRN -> RRN do nó em que as informações serão escritas
    Desloca o ponteiro para a região do nó em que deseja escrever os dados, escreve os dados presentes dentro da struct NO
*/
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



CHAVE_VALOR* insercao(FILE* indice,CHAVE_VALOR* dados, NO* no,ArvBHeader* h, int RRN);
void inserir_chave(CHAVE_VALOR* dados, NO* no);
CHAVE_VALOR* split(CHAVE_VALOR* dados, NO* noesq,int RRNesq, ArvBHeader* h, FILE* indice);
CHAVE_VALOR* aux_insercao_promocao(NO* no, ArvBHeader* h, FILE* indice, int RRN,CHAVE_VALOR* dados, int index);
/* Parâmetros:
    @params nomeBin -> string com o nome do arquivo binário com os registros
    @params nomeIndice -> string com o nome do arquivo binário que irá guardar a árvore B 
    Função que irá executar a funcionalidade de criar um arquivo de índice usando como chave o idAttack e como valores os byteoffsets
    dos registros
*/
bool criar_indice(char* nomeBin, char* nomeIndice){
    //Abertura dos ponteiros para os arquivos
    FILE* bin = fopen(nomeBin,"rb+");
    FILE * indice = fopen(nomeIndice,"wb");//Abre para criar o arquivo

    if(indice == NULL){
        printf("Falha no processamento do arquivo\n");
    }
    fclose(indice);

    indice = fopen(nomeIndice,"rb+");//abre para leitura e escrita
    if(bin == NULL || indice == NULL){
        printf("Falha no processamento do arquivo.\n");
        return false;
    }
    //Verifica se é um arquivo com dados consistentes
    char status;
    fread(&status,sizeof(char),1,bin);

    if(status == '0'){
        printf("Arquivo inconsistente\n");
        return false;
    }
    //Busca o final do arquivo com os registros
    fseek(bin,0,SEEK_END);
    long int final = ftell(bin);
    fseek(bin,0,SEEK_SET);

    //Leitura do header do arquivo com os registros
    HEADER* h = criar_header();
    ler_header(bin,h);

    //Marca o arquivo de indice como inconsistente pois está escrevendo
    fwrite("1",sizeof(char),1,indice);
    fseek(indice,0,SEEK_SET);
    
    //Cria a struct que irá armazenar os dados do header
    ArvBHeader* Bheader = criar_arvB_header();
    //Aloca dinamicamente a struct que irá guardar os dados da raiz
    NO* raiz = criar_no();
    //Inicializa os dados do header
    Bheader->noRaiz = 0;
    Bheader->nroNos++;
    Bheader->proxRRN++;
    //Escreve o nó raiz no arquivo de indice
    escrever_no_indice(indice,raiz,Bheader->noRaiz);
    //loop para percorrer todo o arquivo com os registros
    while(ftell(bin)!=final){
        REG* reg = ler_registro(bin,h);
        //Verifica se o registro é removido
        if(get_removido(reg) == '0'){
            //Criação da chave que será inserida
            CHAVE_VALOR* dados = criar_dados();
            dados->chave = get_idAttack(reg);
            dados->valor = ftell(bin) - get_tamanhoRegistro(reg) -5;
            //Lê o nó raiz e chama a função de insercao
            ler_no_indice(indice,raiz,Bheader->noRaiz);
            insercao(indice,dados,raiz,Bheader,Bheader->noRaiz);
            
        }
        free(reg);
    }
    //Finaliza escrevendo o header e liberando a memória alocada dinâmicamente
    escrever_arvB_header(indice,Bheader);
    free(raiz);
    free(h);
    free(Bheader);
    fclose(bin);
    fclose(indice);
    return true;
}
/*Parâmetros:
    @params indice -> ponteiro para o arquivo de índice
    @params dados -> valores que devem ser inseridos
    @params no -> no que será analisado se pode ocorrer a inserção
    @params RRN -> RRN do no que está sendo analisado se pode ocorrer a inserção
    A função irá verificar se é possível fazer a insercao no nó fornecido. Se não verifica nos nós descendentes, além de
    fazer os splits e reajustes durante as promoções
*/
CHAVE_VALOR* insercao(FILE* indice,CHAVE_VALOR* dados, NO* no,ArvBHeader* h, int RRN){
    //Verifica se é um nó folha para poder inserir
    if(no->tipoNo == -1 && no->nroChaves<2){
        inserir_chave(dados,no);//insere a chave no nó
        escrever_no_indice(indice,no,RRN);//escreve o nó no arquivo
        return NULL;//como não há promoção retorna nulo
    }

    if(no->tipoNo == -1 && no->nroChaves == 2){
        if(RRN == h->noRaiz){//verifica se o nó folha é igual ao nó raiz
            //Realiza o split e adiciona os ponteiros do item que está sendo promovido
            CHAVE_VALOR* promocao = split(dados,no,RRN,h,indice);
    
            //Cria o novo no raiz
            NO* novaRaiz = criar_no();
            novaRaiz->tipoNo = 0;
            inserir_chave(promocao,novaRaiz);

            //Atualiza os valores do header para o no raiz e o proxRRN
            h->noRaiz = h->proxRRN;
            h->proxRRN++;
            h->nroNos++;
            escrever_no_indice(indice,novaRaiz,h->noRaiz);
            NO* oie = criar_no();

            ler_no_indice(indice,oie,h->noRaiz);
            return NULL;
        }
        return split(dados,no,RRN,h,indice);// retorna a chave promovida após o split
    }
    
    //Verifica se é nó raiz e chama a função que irá tratar da recursão e a manipulação da promoção na volta da recusão
    if(no->tipoNo == 0){
         if(no->chaves[0]> dados->chave){
            return aux_insercao_promocao(no,h,indice,RRN,dados,0);
            
        }
        else if(no->chaves[1]>dados->chave || no->chaves[1] == -1){
            return aux_insercao_promocao(no,h,indice,RRN,dados,1);
            
        }
        else{
            return aux_insercao_promocao(no,h,indice,RRN,dados,2);
        }
    }
    //Verifica se é o nó folha e chama a função auxiliar para a recursão
    if(no->tipoNo == 1){
        if(no->chaves[0]> dados->chave){
            return aux_insercao_promocao(no,h,indice,RRN,dados,0);
        }
        else if(no->chaves[1]>dados->chave || no->chaves[1] == -1){
            return aux_insercao_promocao(no,h,indice,RRN,dados,1);
        }
        else{
            return aux_insercao_promocao(no,h,indice,RRN,dados,2);
        }

    }
    
    return NULL;
}
/*Parâmetros:
    @params dados -> chave e valor a serem inseridos 
    @params no -> nó em que deve ocorrer a inserção
    Executa a insercao e ajustes dos ponteiros do nó em que ocorre a insercao
*/
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
/*Parâmetros:
    @params dados -> dados a serem inseridos
    @params noesq -> nó cheio em que o dado quer ser inserido
    @params RRNesq-> RRN no nó cheio
    @params h -> ponteiro pra o header do arquivo de indice
    @prams indice -> ponteiro para o arquivo de indice
    Função que faz a criação de um novo nó e distribuir de os dados do nó antigo entre eles e retorna a chave promovida no
    processo de split.
*/
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
        //Ajuste dos ponteiros da promoção
        promocao->esq = RRNesq;
        promocao->dir = h->proxRRN;
        //ajuste dos dados do header
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
        //Ajuste dos ponteiros da promocao
        promocao->esq = RRNesq;
        promocao->dir = h->proxRRN;
        //ajuste dos dados do header
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
        //Ajuste dos ponteiros da promoção
        promocao->esq = RRNesq;
        promocao->dir = h->proxRRN;
        //Ajuste dos dados do header
        h->proxRRN++;
        h->nroNos++;
        free(nodir);
        return promocao;

    }
    free(nodir);
    return NULL;

}
/*Parâmetros:
    @params no -> struct do nó em que se tenta inserir
    @params h -> ponteiro para struct com os dados do header
    @params indice -> ponteiro para o arquivo de indice
    @params RRN -> RRN do nó atual
    @params dados -> dados que deseja-se inserir no nó
    @params index -> qual descendente será chamado para poder fazer a recursão
    A função irá auxiliar na chamada da recursão para o descendente correto, além de auxiliar na volta da recursão em que
    pode se ter uma promocao.
    O retorno da função será a promoção que pode ser necessário subir para o próximo nível da recursão
*/
CHAVE_VALOR* aux_insercao_promocao(NO* no, ArvBHeader* h, FILE* indice, int RRN,CHAVE_VALOR* dados, int index){
    NO* descendente = criar_no();
    ler_no_indice(indice,descendente,no->descendentes[index]);
    CHAVE_VALOR* promocao = insercao(indice,dados,descendente,h,no->descendentes[index]);
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
/*
    Percorre a árvore procurando o nó que contenha a chave igual ao id desejado
*/
void busca_arvore(char* nomebin, char* nomeindice,char* campos[],char*valores[],int tamanho,int id){
    //Abre os arquivos
    FILE* bin = fopen(nomebin,"rb+");
    FILE* indice = fopen(nomeindice,"rb+");

    if(bin == NULL || indice == NULL ){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //Crialçao do header e da raiz em memória principal
    ArvBHeader* h = criar_arvB_header();
    ler_arvB_header(indice,h);
    NO* raiz = criar_no();
    ler_no_indice(indice,raiz,h->noRaiz);
    //busca e retorna a posição do registro
    long int posicaoReg = busca_id_arvB(indice,id,raiz);
    //le o header do arquivo de registros
    HEADER* headerReg = criar_header();
    ler_header(bin,headerReg);
    //Se o retorno da posicao for valido, vai até a posicao e mostra os dados do registro
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
/*Parâmetros:
    @params indice -> ponteiro para o arquivo de indice
    @params id -> id que se deseja procurar
    @params no -> no atual que está sendo procurado o id
Faz a parte da recursão e descida pela arvore a partir do nó raiz
*/
long int busca_id_arvB(FILE* indice,int id,NO* no){
    //Verifica se já chegou em um no raiz sem o id
    if(no->tipoNo == -1 && no->chaves[0] != id && no->chaves[1] != id){
        return -1;
    }

    if(no->chaves[0] == id){
        return no->registros[0];
    }

    if(no->chaves[1] == id){
        return no->registros[1];
    }
    //Parte da recursão descendo pela árvore
    NO* descendente = criar_no();
    long int resultado = -1;
    //casos de descendentes que devem percorrer, se pela esquerda pelo meio ou pela direita
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
/*Parâmetros:
    @params nomebin -> nome do arquivo com os registros
    @params nomeindice -> nome do arquivo de indice
    @params valores -> byteofffsets dos registros inseridos
    @params tamanho -> quantidade de registros inseridos
    Com o byteoffset dos registros inseridos é possível encontrar o id deles e inserir o id e o byteoffset dentro do arquivo
    de indice. Os byteoffsets estão ordenado por ondem de inserção ou seja valores[0] foi inserido primeiro que valores[1] e
    assim por diante.
*/
void ajuste_indice_inserInto(char* nomebin,char* nomeindice,long int *valores,int tamanho){
    //abertura dos arquivos
    FILE* bin = fopen(nomebin,"rb+");
    FILE* indice = fopen(nomeindice,"rb+");

    if(bin == NULL || indice == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //Le o header do arquivo de registros
    HEADER* binHeader = criar_header();
    ler_header(bin, binHeader);
    //Lê o header do arquivo de indice
    ArvBHeader * indiceHeader = criar_arvB_header();
    ler_arvB_header(indice,indiceHeader);
    //coloca o arquivo como inconsistente
    fseek(indice,0,SEEK_SET);
    fwrite("1",sizeof(char),1,indice);

    NO* raiz = criar_no();//struct para o nó raiz
    //insercao no arquivo de indice
    for(int i = 0;i<tamanho;i++){
        //Cria a struct CHAVE VALOR que deve ser inserida no arquivo de indice
        fseek(bin,valores[i],SEEK_SET);
        REG* reg = ler_registro(bin,binHeader);
        CHAVE_VALOR * novaChave = criar_dados();
        novaChave->chave = get_idAttack(reg);
        novaChave->valor = valores[i];
        //Chamada da insercao
        ler_no_indice(indice,raiz,indiceHeader->noRaiz);
        insercao(indice,novaChave,raiz,indiceHeader,indiceHeader->noRaiz);
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
        //remover_registro(bin, reg_atual, header, posicao);
        
        // Insere o novo registro
        long int nova_posicao = inserir_registro_final(bin, header, reg_atualizado);
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
    
    // Marca os arquivos como consistentes
    set_status(headerDados, '1');
    escreverCabecalho(bin, headerDados);
    headerIndice->status = '1';
    escrever_arvB_header(indice, headerIndice);
    
    // Chama binarioNaTela para mostrar os arquivos
    binarioNaTela(nomeBin);
    binarioNaTela(nomeIndice);
    
    // Libera memória e fecha arquivos
    free(headerDados);
    free(headerIndice);
    fclose(bin);
    fclose(indice);
}