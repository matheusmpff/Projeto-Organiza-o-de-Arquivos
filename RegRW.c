#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "RegRW.h"

void ajuste_linha(char *linha);
void ajuste(char* palavra);

/*
A struct header é feita para armazenar os dados do header do arquvio binário. 
Com isso os campos sõa criados para representar os dados que estão no header.
 */
struct header{
    char status;
    long int topo;
    long int proxByteOffset;
    int nroReqArq;
    int nroReqRem;
    char descreveIdentificador [24];
    char descreveYear[28];
    char descreverFinancialLoss[29];
    char codDescreveCountry;
    char descreveCountry[27];
    char codDescreveType;
    char descreveType[39];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[39];
    char codDescreveDefense;
    char descreveDefense[68];
};
/*A struct REG é feita para simular os registros que estarão presentes no arquivo binário, 
apresentam os campos pra os valores fixos e variáveis. Os campos variáveis terão keywords que
 serão processadas durante a leitura do arquivo binário.
*/
struct reg{
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



void escrever_cabecalho(FILE* fp, FILE* bin, HEADER* header){//NOVO
    //ESCREVE LINHA DO CABEÇALHO
    char linha[256];
    char * aux;
    fgets(linha,sizeof(linha),fp);
    
    ajuste(linha);
    aux = strtok(linha,",");
    strcpy(header->descreveIdentificador,aux);
    aux = strtok(NULL,",");
    strcpy(header->descreveYear,aux);
    aux = strtok(NULL,",");
    strcpy(header->descreverFinancialLoss,aux);
    aux = strtok(NULL,",");
    strcpy(header->descreveCountry,aux);
    aux = strtok(NULL,",");
    strcpy(header->descreveType,aux);
    aux = strtok(NULL,",");
    strcpy(header->descreveTargetIndustry,aux);
    aux = strtok(NULL,",");
    strcpy(header->descreveDefense,aux);
    
    fwrite(&header->status,sizeof(char),1,bin);
    fwrite(&header->topo,sizeof(long int),1,bin);
    fwrite(&header->proxByteOffset,sizeof(long int),1,bin);
    fwrite(&header->nroReqArq,sizeof(int),1,bin);
    fwrite(&header->nroReqRem,sizeof(int),1,bin);
    fwrite(header->descreveIdentificador,strlen(header->descreveIdentificador),1,bin);
    fwrite(header->descreveYear,strlen(header->descreveYear),1,bin);
    fwrite(header->descreverFinancialLoss,strlen(header->descreverFinancialLoss),1,bin);
    fwrite(&header->codDescreveCountry,sizeof(char),1,bin);
    fwrite(header->descreveCountry,strlen(header->descreveCountry),1,bin);
    fwrite(&header->codDescreveType,sizeof(char),1,bin);
    fwrite(header->descreveType,strlen(header->descreveType),1,bin);
    fwrite(&header->codDescreveTargetIndustry,sizeof(char),1,bin);
    fwrite(header->descreveTargetIndustry,strlen(header->descreveTargetIndustry),1,bin);
    fwrite(&header->codDescreveDefense,sizeof(char),1,bin);
    fwrite(header->descreveDefense,strlen(header->descreveDefense),1,bin);

}

int get_status(HEADER * h){
    return h->status;
}
bool set_status(HEADER * h,int x){
    h->status = x;
    return true;
}
int get_topo(HEADER *h){
    return h->topo;
}
bool set_topo(HEADER *h, int x){
    h->topo = x;
    return true;
}
int get_proxByteOffset(HEADER *h){
    return h->proxByteOffset;
}
bool set_proxByteOffset(HEADER *h){
    return h->proxByteOffset;
}
int get_nroReqArq(HEADER *h){
    return h->nroReqArq;
}
bool set_nroReqArq(HEADER *h,int x){
    h->nroReqArq = x;
    return true;
}
int get_nroReqRem(HEADER *h){
    return h->nroReqRem;
}
bool set_nroReqRem(HEADER *h, int x){
    h->nroReqRem = x;
    return true;
}
char * get_descreveident(HEADER *h){
    return h->descreveIdentificador;
}
bool set_descreveident(HEADER *h,char * src){
    strcpy(h->descreveIdentificador,src);
    return true;
}
char * get_descreveYear(HEADER *h){
    return h->descreveYear;
}
bool set_descreveYear(HEADER *h, char*src){
    strcpy(h->descreveYear,src);
    return true;
}
char * get_descreveFl(HEADER * h){
    return h->descreverFinancialLoss;
}
bool set_descreveFl(HEADER *h, char * src){
    strcpy(h->descreverFinancialLoss,src);
    return true;
}
char *get_descreveCountry(HEADER *h){
    return h->descreveCountry;
}
bool set_descreveCountry(HEADER * h,char * src){
    strcpy(h->descreveCountry,src);
    return true;
}
char *get_descreveType(HEADER *h){
    return h->descreveType;
}
bool set_descreveType(HEADER *h, char* src){
    strcpy(h->descreveType,src);
    return true;
}
char * get_descreveTarget(HEADER *h){
    return h->descreveTargetIndustry;
}
bool set_descreveTarget(HEADER *h, char* src){
    strcpy(h->descreveTargetIndustry,src);
    return true;
}
char * get_descreveDefense(HEADER *h){
    return h->descreveDefense;
}
bool set_descreveDefense(HEADER *h, char* src){
    strcpy(h->descreveDefense,src);
    return true;
}

REG* criar_reg(){//NOVO
    REG * reg = (REG*) calloc(sizeof(REG),1);
    if(reg == NULL){
        printf("Problema na criação do registro\n");
        return reg;
    }
    reg-> removido = '0';
    reg->tamanhoRegistro = 20;
    reg->prox = -1;
    reg->idAttack = -10;
    reg->year = -10;
    reg->financialLoss= -10;
    strcpy(reg->country,"-1000");
    strcpy(reg->attackType,"-1000");
    strcpy(reg->targetIndustry,"-1000");
    strcpy(reg->defenseMechanism,"-1000");

    return reg;
}

bool ler_linha(FILE* fp, REG *reg){
    char* aux;
    char linha [256];
    reg->tamanhoRegistro = 20;
    if(fgets(linha,sizeof(linha),fp) == NULL){
        return false;
    }
    ajuste(linha);
    ajuste_linha(linha);
    aux = strtok(linha,",");
    if(aux == NULL){
        reg->idAttack = -1;
    }
    else{
        reg->idAttack = atoi(aux);
    }
    
    aux = strtok(NULL,",");
    if(aux == NULL){
        reg->year = -1;
    }
    else{
        reg->year = atoi(aux);
    }
    aux = strtok(NULL,",");
    if(aux == NULL){
        reg->financialLoss= -1;
    }
    else{
        reg->financialLoss= atof(aux);
    }
    aux = strtok(NULL,",");
    if(aux == NULL){
        strcpy(reg->country,"-1");
    }
    else{
        ajuste(aux);
        strcpy(reg->country,aux);
    }
    
    aux = strtok(NULL,",");
    if(aux == NULL){
        strcpy(reg->attackType,"-1");
    }
    else{
        ajuste(aux);
        strcpy(reg->attackType,aux);
    }
    aux = strtok(NULL,",");
    if(aux == NULL){
        strcpy(reg->targetIndustry,"-1");
    }
    else{
        ajuste(aux);
        strcpy(reg->targetIndustry,aux);
    }
    aux = strtok(NULL,",\n");
    if(aux == NULL){
        strcpy(reg->defenseMechanism,"-1");
    }
    else{
        ajuste(aux);
        strcpy(reg->defenseMechanism,aux);
    }
      
    if(strcmp(reg->country,"-1")){
        reg->tamanhoRegistro += (strlen(reg->country)+2);
    }
    if(strcmp(reg->attackType,"-1")){
        reg->tamanhoRegistro += (strlen(reg->attackType)+2);
    }
    if(strcmp(reg->targetIndustry,"-1")){
        reg->tamanhoRegistro += (strlen(reg->targetIndustry)+2);
    }
    if(strcmp(reg->defenseMechanism,"-1")){
        reg->tamanhoRegistro += (strlen(reg->defenseMechanism)+2);
    }

    return true;
 
}

void escrever_registros(FILE* fp, FILE* bin, REG* reg){
    while(ler_linha(fp,reg)){

        fwrite(&reg->removido,sizeof(char),1,bin);
        fwrite(&reg->tamanhoRegistro,sizeof(int),1,bin);
        fwrite(&reg->prox,sizeof(long int),1,bin);
        fwrite(&reg->idAttack,sizeof(int),1,bin);
        fwrite(&reg->year,sizeof(int),1,bin);
        fwrite(&reg->financialLoss,sizeof(float),1,bin);
        if(strcmp(reg->country,"-1")){
            fwrite("1",sizeof(char),1,bin);
            fwrite(reg->country,strlen(reg->country),1,bin);
            fwrite("|",sizeof(char),1,bin);
        }
        if(strcmp(reg->attackType,"-1")){
            fwrite("2",sizeof(char),1,bin);
            fwrite(reg->attackType,strlen(reg->attackType),1,bin);
            fwrite("|",sizeof(char),1,bin);
        }
        if(strcmp(reg->targetIndustry,"-1")){
            fwrite("3",sizeof(char),1,bin);
            fwrite(reg->targetIndustry,strlen(reg->targetIndustry),1,bin);
            fwrite("|",sizeof(char),1,bin);
        }
        if(strcmp(reg->defenseMechanism,"-1")){
            fwrite("4",sizeof(char),1,bin);
            fwrite(reg->defenseMechanism,strlen(reg->defenseMechanism),1,bin);
            fwrite("|",sizeof(char),1,bin);
        }
    }
}

char get_removido(REG *r){
    return r->removido;
}

int get_idAttack(REG *r){
    return r->idAttack;
}

int get_year(REG *r){
    return r->year;
}

float get_financialLoss(REG *r){
    return r->financialLoss;
}

char* get_country(REG *r){
    return r->country;
}

char* get_attackType(REG *r){
    return r->attackType;
}

char* get_targetIndustry(REG *r){
    return r->targetIndustry;
}

char* get_defenseMechanism(REG *r){
    return r->defenseMechanism;
}

void add_lixo(int tam, char *vet){
    for(int i = 0;i<tam;i++){
        vet[i] = '$';
    }
}

void aux_ler_registro(FILE* bin, REG * reg, HEADER* h){//NOVO
    int tam = reg->tamanhoRegistro;
    char auxChar = '\0';

    while(tam>20){
        
        fread(&auxChar,sizeof(char),1,bin);
        if(auxChar == h->codDescreveCountry){
            int i = 0;
            while(auxChar != '|'){ 
                fread(&auxChar,sizeof(char),1,bin);
                reg->country[i] = auxChar;
                i++;
            }
            reg->country[i-1] = '\0';
            tam -= (strlen(reg->country) +2); 
        }
        if(auxChar == h->codDescreveType){
            int i = 0;
            while(auxChar != '|'){ 
                fread(&auxChar,sizeof(char),1,bin);
                reg->attackType[i] = auxChar;
                i++;
            }
            reg->attackType[i-1] = '\0';
            tam -= ( strlen(reg->attackType) +2); 
        }
        if(auxChar == h->codDescreveTargetIndustry){
            int i = 0;
            while(auxChar != '|'){ 
                fread(&auxChar,sizeof(char),1,bin);
                reg->targetIndustry[i] = auxChar;
                i++;
            }
            reg->targetIndustry[i-1] = '\0';
            tam -= ( strlen(reg->targetIndustry) +2);
        }
        if(auxChar == h->codDescreveDefense){
            int i = 0;
            while(auxChar != '|'){ 
                fread(&auxChar,sizeof(char),1,bin);
                reg->defenseMechanism[i] = auxChar;
                i++;
            }
            reg->defenseMechanism[i-1] = '\0';
            tam -= ( strlen(reg->defenseMechanism) +2);
        }
    }
}

REG* ler_registro(FILE* bin,HEADER *h){
    REG* reg = criar_reg();
    fread(&reg->removido,sizeof(char),1,bin);
    fread(&reg->tamanhoRegistro,sizeof(int),1,bin);
    fread(&reg->prox,sizeof(long int),1,bin);
    fread(&reg->idAttack,sizeof(int),1,bin);
    fread(&reg->year,sizeof(int),1,bin);
    fread(&reg->financialLoss,sizeof(float),1,bin);


    aux_ler_registro(bin, reg,h);

    return reg;
}

/*-------------------------------- adições novas------------------------------------------*/


//FUNÇÕES DE AJUSTE DE LINHA
void ajuste_linha(char *linha) {
   char resultado[300]="";
    int i;
    int j = 0;

    if(linha[0] == ','){
        resultado[j] = '-';
        j++;
        resultado[j] = '1';
        j++;
    }
    for(i=0;linha[i]!='\0';i++){
        

        if(linha[i] == linha[i+1] && linha[i] == ','){

            resultado[j] = linha[i];
            j++;
            resultado[j] = '-';
            j++;
            resultado[j] = '1'; 
            j++;
            continue;
        }
        resultado[j] = linha[i];
        j++;
    }

    if(resultado[j-1] == ','){
        resultado[j] = '-';
        j++;
        resultado[j] = '1';
        j++;
    }

    resultado[j] = '\0';
    strcpy(linha,resultado);
}

void ajuste(char* palavra){
    if(palavra == NULL){
        return;
    }
    
    for(int i = 0;i<2;i++){
        int tam = strlen(palavra);
        if(palavra[tam-1] =='\n' || palavra[tam-1] == '\r' || palavra[tam-1] == ' '){
            palavra[tam-1] = '\0';
        } 
    }
    

}

//criar header

HEADER* criar_header(){
    HEADER * header =(HEADER*) calloc(sizeof(HEADER),1);
    if(header == NULL){
        printf("Erro na alocação de memória");
        return header;
    }

    header->status = '1';
    header->topo = -1;
    header->proxByteOffset = 0;
    header->nroReqArq = 0;
    header->nroReqRem = 0;
    header->codDescreveCountry = '1';
    header->codDescreveType = '2';
    header->codDescreveTargetIndustry = '3';
    header->codDescreveDefense = '4';
    strcpy(header->descreveIdentificador,"-1");
    strcpy(header->descreveYear,"-1");
    strcpy(header->descreverFinancialLoss,"-1");
    strcpy(header->descreveCountry,"-1");
    strcpy(header->descreveType,"-1");
    strcpy(header->descreveTargetIndustry,"-1");
    strcpy(header->descreveDefense,"-1");

    return header;
}


//Ler Header
void ler_header(FILE* bin, HEADER* h){
    
    fread(&h->status,sizeof(char),1,bin);
    fread(&h->topo,sizeof(long int),1,bin);
    fread(&h->proxByteOffset,sizeof(long int),1,bin);
    fread(&h->nroReqArq,sizeof(int),1,bin);
    fread(&h->nroReqRem,sizeof(int),1,bin);
    fread(h->descreveIdentificador,sizeof(char),23,bin);
    h->descreveIdentificador[23] = '\0';
    fread(h->descreveYear,sizeof(char),27,bin);
    h->descreveYear[27] = '\0';
    fread(h->descreverFinancialLoss,sizeof(char),28,bin);
    h->descreverFinancialLoss[28] = '\0';
    fread(&h->codDescreveCountry,sizeof(char),1,bin);
    fread(h->descreveCountry,sizeof(char),26,bin);
    h->descreveCountry[26] = '\0';
    fread(&h->codDescreveType,sizeof(char),1,bin);
    fread(h->descreveType,sizeof(char),38,bin);
    h->descreveType[38] = '\0';
    fread(&h->codDescreveTargetIndustry,sizeof(char),1,bin);
    fread(h->descreveTargetIndustry,sizeof(char),38,bin);
    h->descreveTargetIndustry[38] = '\0';
    fread(&h->codDescreveDefense,sizeof(char),1,bin);
    fread(h->descreveDefense,sizeof(char),67,bin);
    h->descreveDefense[67] = '\0';
    

}


//printa o header
void printar_header(HEADER* h){
    printf("Status: %c\n",h->status);
    printf("Topo: %ld\n",h->topo);
    printf("ProxByteOffset: %ld\n",h->proxByteOffset);
    printf("NRORefArq: %d\n",h->nroReqArq);
    printf("NRORegRem: %d\n",h->nroReqRem);
    printf("DescreveIdentidicador: %s\n",h->descreveIdentificador);
    printf("DescreveYear: %s\n",h->descreveYear);
    printf("DescreveFinancialLoss: %s\n",h->descreverFinancialLoss);
    printf("CodDescreveCountry: %c\n",h->codDescreveCountry);
    printf("DescreveCountry: %s\n",h->descreveCountry);
    printf("CodDescreveType: %c\n",h->codDescreveType);
    printf("DescreveType: %s\n",h->descreveType);
    printf("CodDesceveTargetIndustry: %c\n",h->codDescreveTargetIndustry);
    printf("DescreveTargetIndustry: %s\n",h->descreveTargetIndustry);
    printf("CodDescreveDefense: %c\n",h->codDescreveDefense);
    printf("DescreveDefense: %s\n",h->descreveDefense);


}


//PRINTAR REGISTROS

void printar_registro(REG* reg,HEADER* h){
    
    if(reg->idAttack != -1){
        printf("%s: %d\n",h->descreveIdentificador,reg->idAttack);
    }
    else{
        printf("%s: NADA CONSTA\n",h->descreveIdentificador);
    }
    

    if(reg->year != -1){
        printf("%s: %d\n",h->descreveYear,reg->year);
    }
    else{
        printf("%s: NADA CONSTA\n",h->descreveYear);
    }

    if(strcmp(reg->country,"-1000")){
        printf("%s: %s\n",h->descreveCountry,reg->country);
    }
    else{
        printf("%s: NADA CONSTA\n",h->descreveCountry);
    }

    if(strcmp(reg->targetIndustry,"-1000")){
        printf("%s: %s\n",h->descreveTargetIndustry,reg->targetIndustry);
    }
    else{
        printf("%s: NADA CONSTA\n",h->descreveTargetIndustry);
    }

    if(strcmp(reg->attackType,"-1000")){
        printf("%s: %s\n",h->descreveType,reg->attackType);
    }
    else{
        printf("%s: NADA CONSTA\n",h->descreveType);
    }


    if(reg->financialLoss!=-1){
        printf("%s: %.2f\n",h->descreverFinancialLoss,reg->financialLoss);
    }
    else{
        printf("%s: NADA CONSTA\n",h->descreverFinancialLoss);
    }

    if(strcmp(reg->defenseMechanism,"-1000")){
        printf("%s: %s\n",h->descreveDefense,reg->defenseMechanism);
    }
    else{
        printf("%s: NADA CONSTA\n",h->descreveDefense);
    }

    printf("\n");
    
}
//printa todos os registros de um arquivo binario
void printar_binario(char * nome){
    FILE * bin = fopen(nome,"rb");
    if(bin == NULL){
       printf("Falha no processamento do arquivo.\n");
       return;
    }

    fseek(bin,0,SEEK_END);
    int final = ftell(bin);
    fseek(bin,0,SEEK_SET);

    HEADER* h = criar_header();
    ler_header(bin,h);
    printar_header(h);
    int i = 0;
   /* while(ftell(bin) != final){
        REG* reg = ler_registro(bin,h);
        printar_registro(reg,h);
        i++;
        free(reg);
    }*/
    
    
    free(h);
    fclose(bin);
}