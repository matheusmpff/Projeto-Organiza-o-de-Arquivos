#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "RegRW.h"

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

HEADER* create_header(HEADERPARAMS params){

    HEADER* h;
    h =(HEADER*) malloc(sizeof(HEADER)*1);
    h->status = params.status;
    h->topo = params.topo;
    h->proxByteOffset = params.proxByteOffset;
    h->nroReqArq = params.nroReqArq;
    h->nroReqRem = params.nroReqRem;
    h->codDescreveCountry = '1';
    h->codDescreveType= '2';
    h->codDescreveTargetIndustry = '3';
    h->codDescreveDefense = '4';
    strcpy(h->descreveCountry,params.descreveCountry);
    strcpy(h->descreveDefense,params.descreveDefense);
    strcpy(h->descreveIdentificador,params.descreveIdentificador);
    strcpy(h->descreverFinancialLoss,params.descreverFinancialLoss);
    strcpy(h->descreveTargetIndustry,params.descreveTargetIndustry);
    strcpy(h->descreveType,params.descreveType);
    strcpy(h->descreveYear,params.descreveYear);

    return h;
}

bool escrever_header(HEADER* h, FILE* fp){
    fwrite(&h->status,sizeof(char),1,fp);
    fwrite(&h->topo,sizeof(long int),1,fp);
    fwrite(&h->proxByteOffset,sizeof(long int),1,fp);
    fwrite(&h->nroReqArq,sizeof(int),1,fp);
    fwrite(&h->nroReqRem,sizeof(int),1,fp);
    fwrite(h->descreveIdentificador,sizeof(char),23,fp);
    fwrite(h->descreveYear,sizeof(char),27,fp);
    fwrite(h->descreverFinancialLoss,sizeof(char),28,fp);
    fwrite(&h->codDescreveCountry,sizeof(char),1,fp);
    fwrite(&h->descreveCountry,sizeof(char),26,fp);
    fwrite(&h->codDescreveType,sizeof(char),1,fp);
    fwrite(&h->descreveType,sizeof(char),38,fp);
    fwrite(&h->codDescreveTargetIndustry,sizeof(char),1,fp);
    fwrite(&h->descreveTargetIndustry,sizeof(char),38,fp);
    fwrite(&h->codDescreveDefense,sizeof(char),1,fp);
    fwrite(&h->descreveDefense,sizeof(char),67,fp);

    return true;

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






REG_DADOS* criar_regDados(REGPARAMS params){
    REG_DADOS* reg;

    reg = (REG_DADOS*) malloc(sizeof(REG_DADOS)*1);

    reg->removido = params.removido; 
    reg->tamanhoRegistro = params.tamanhoRegistro;
    reg->prox = params.prox;
    reg->year = params.year;
    reg->idAttack = params.idAttack;
    reg->financialLoss = params.financialLoss;
    strcpy(reg->targetIndustry,params.targetIndustry);
    strcpy(reg->defenseMechanism,params.defenseMechanism);
    strcpy(reg->attackType,params.attackType);
    strcpy(reg->country,params.country);

    return reg;
}

char get_removido(REG_DADOS *r){
    return r->removido;
}

int get_idAttack(REG_DADOS *r){
    return r->idAttack;
}

int get_year(REG_DADOS *r){
    return r->year;
}

float get_financialLoss(REG_DADOS *r){
    return r->financialLoss;
}

char* get_country(REG_DADOS *r){
    return r->country;
}

char* get_attackType(REG_DADOS *r){
    return r->attackType;
}

char* get_targetIndustry(REG_DADOS *r){
    return r->targetIndustry;
}

char* get_defenseMechanism(REG_DADOS *r){
    return r->defenseMechanism;
}

void add_lixo(int tam, char *vet){
    for(int i = 0;i<tam;i++){
        vet[i] = '$';
    }
}

bool escrever_regDados(FILE* fp,REG_DADOS *reg){

    
    fwrite(&reg->removido,sizeof(char),1,fp);
    fwrite(&reg->tamanhoRegistro,sizeof(int),1,fp);
    fwrite(&reg->prox,sizeof(long int),1,fp);
    fwrite(&reg->idAttack,sizeof(int),1,fp);
    fwrite(&reg->year,sizeof(int),1,fp);
    fwrite(&reg->financialLoss,sizeof(float),1,fp);
    
    char aux = '|';

    char flag = '1';
    if(reg->country[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->country[i]!='\0';i++){
        fwrite(&reg->country[i],sizeof(char),1,fp);
    }
    if(reg->country[0]!='\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }
    
    flag = '2';
    if(reg->attackType[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->attackType[i]!='\0';i++){
        fwrite(&reg->attackType[i],sizeof(char),1,fp);
    }
    if(reg->attackType[0] != '\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }
    
    flag = '3';
    if(reg->targetIndustry[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->targetIndustry[i]!='\0';i++){
        fwrite(&reg->targetIndustry[i],sizeof(char),1,fp);
    }
    if(reg->targetIndustry[0]!= '\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }
   
    flag = '4';
    if(reg->defenseMechanism[0]!='\0'){fwrite(&flag,sizeof(char),1,fp);}
    for(int i = 0; reg->defenseMechanism[i]!='\0';i++){
        fwrite(&reg->defenseMechanism[i],sizeof(char),1,fp);
    }
    if(reg->defenseMechanism[0]!='\0'){
        fwrite(&aux,sizeof(char),1,fp);
    }

    fclose(fp);
    return true;
    
}

void ler_campos_variaveis(FILE *fp,REG_DADOS *reg){

    char aux;
    int tam = reg->tamanhoRegistro;

    while(tam > 25){
        fread(&aux,sizeof(char),1,fp);
        int i = 0;
        if(aux == '1'){
            do{
                fread(&reg->country[i],sizeof(char),1,fp);
                i++;
            }
            while(reg->country[i-1]!='|');
            reg->country[i-1] = '\0';
            tam = tam - strlen(reg->country) - 1; 
            printf("%s",reg->country);
            //printf("%d\n",tam);
        }
        
        i = 0;
        if(aux == '2'){
            do{
                fread(&reg->attackType[i],sizeof(char),1,fp);
                i++;
            }
            while(reg->attackType[i-1]!='|'); 
            reg->attackType[i-1] = '\0';
            tam = tam - strlen(reg->attackType) - 1;
            //printf("%d\n",tam);   
        }
        
        i=0;
        if(aux == '3'){
            do{
                fread(&reg->targetIndustry[i],sizeof(char),1,fp);
                i++;
            }
            while(reg->targetIndustry[i-1]!='|');
            reg->targetIndustry[i-1] = '\0';
            tam = tam - strlen(reg->targetIndustry) - 1; 
            //printf("%d\n",tam);     
        }
        
        i=0;
        if(aux == '4'){
            do{
                fread(&reg->defenseMechanism[i],sizeof(char),1,fp);
                i++;
            }
            while(reg->defenseMechanism[i-1]!='|');
            reg->defenseMechanism[i-1] = '\0';
            tam = tam - strlen(reg->defenseMechanism) - 1;
            //printf("%d\n",tam);    
        }
    }
     
}

REG_DADOS* ler_regDados(FILE *fp){
    REGPARAMS params;
    REG_DADOS *reg = criar_regDados(params);
    fread(&reg->removido,sizeof(char),1,fp);
    fread(&reg->tamanhoRegistro,sizeof(int),1,fp);
    fread(&reg->prox,sizeof(long int),1,fp);
    fread(&reg->idAttack,sizeof(int),1,fp);
    fread(&reg->year,sizeof(int),1,fp);
    fread(&reg->financialLoss,sizeof(float),1,fp);
    
    if(reg->tamanhoRegistro > 25){
        ler_campos_variaveis(fp, reg);
    }
    
    

    return reg;
}

void printt_reg(REG_DADOS* reg){

    printf("removido: %c\n",reg->removido);
    printf("tamanhoReg: %d\n",reg->tamanhoRegistro);
    printf("prox: %ld\n",reg->prox);
    printf("idAttack: %d\n",reg->idAttack);
    printf("year: %d\n",reg->year);
    printf("financial loss: %f\n",reg->financialLoss);
    printf("country: %s\n",reg->country);
    printf("attacktype: %s\n",reg->attackType);
    printf("defense: %s\n",reg->defenseMechanism);
    printf("target: %s\n",reg->targetIndustry);
}