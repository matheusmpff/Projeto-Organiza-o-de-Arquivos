
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "remocao.h"
#include "insertInto.h"

#define TamMax 1000



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

    for(int i = 0;i<1;i++){
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

    fwrite(header->status,sizeof(char),1,bin);
    fwrite(header->noRaiz,sizeof(int),1,bin);
    fwrite(header->proxRRN,sizeof(int),1,bin);
    fwrite(header->nroNos,sizeof(int),1,bin);
    
    for(int i = 0;i<31;i++){
        fwrite('$',sizeof(char),1,bin);
    }
}

bool criar_indice(char* nomeBin, char* nomeIndice){

    FILE* bin = fopen(nomeBin,"rb+");
    FILE * indice = fopen(nomeIndice,"rb+");

    if(bin == NULL || indice == NULL){
        printf("Flaha no processamento do arquivo.\n");
        return false;
    }

    char status;
    fread(&status,sizeof(int),1,bin);

    if(status == '0'){
        printf("Arquivo inconsistente\n");
        return false;
    }

    int final = fseek(bin,0,SEEK_END);
    fseek(bin,276,SEEK_SET);
    while(fteel(bin) < final){
        
    }


    
    fclose(bin);
    fclose(indice);
    return true;
}

