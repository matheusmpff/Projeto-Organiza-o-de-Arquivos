#include <stdio.h>
#include "RegRW.h"
#include <string.h>
#include <stdlib.h>

/*Função para converter um arquvio csv para binário, a primeira parte do programa compila 
lé a primeira linha do arquivo para pegar os dados necessários para criar o header. Após a
 coleta dos dados para o header que tem um tamanho fixo sempre, começa a leitura dos dados
  para serem criados os registros do arquivo binário

Para a leitura de dados númericos foi usado o fscanf para poder converter os dados vindos do 
arquivo texto para seus respectivos tipos de dados. Após a leitura dos dados númericos, começa
a leitura dos dados textuais, os quais também apresentam tamanho variado. Com isso, utilizou-se
o fread para poder ter controle byte a byte do que está sendo lido e não ter erro de leitura e 
nem ter erro na parte de adicionar o \0 ao final das Strings. 
*/
bool csv_to_bin(char* arqcsv,char*arqbin){
    FILE *fp;
    FILE* bin;
    HEADERPARAMS params_h;

    fp = fopen(arqcsv,"r");
    bin = fopen(arqbin,"wb");
    

    if(fp == NULL || bin == NULL){
        printf("Problema na abertura do arquivo");
        return false;
    }
    fseek(fp,0,SEEK_END);
    long int fim_csv = ftell(fp);
    fseek(fp,0,SEEK_SET);
    
    //inicialização dos parâmetros do header
    params_h.status = '0';
    params_h.topo = -1;
    params_h.proxByteOffset = 0;
    params_h.nroReqArq = 0;
    params_h.nroReqRem = 0;

    fread(params_h.descreveIdentificador,sizeof(char),24,fp);
    params_h.descreveIdentificador[23] = '\0';
    fread(params_h.descreveYear,sizeof(char),28,fp);
    params_h.descreveYear[27] = '\0';
    fread(params_h.descreverFinancialLoss,sizeof(char),29,fp);
    params_h.descreverFinancialLoss[28] = '\0';
    fread(params_h.descreveCountry,sizeof(char),27,fp);
    params_h.descreveCountry[27] = '\0';
    fread(params_h.descreveType,sizeof(char),39,fp);
    params_h.descreveType[38] = '\0';
    fread(params_h.descreveTargetIndustry,sizeof(char),39,fp);
    params_h.descreveTargetIndustry[38]='\0';
    fread(params_h.descreveDefense,sizeof(char),68,fp);
    params_h.descreveDefense[67]='\0';
    
    
    fseek(bin,276,SEEK_SET); //Pula o espaço do cabeçalho pois ele será escrito ao final quando já tiver colocado todos os registros


    int n = 0;
    //Parte de leitura de todo arquivo csv e passagem dos dados para o binário
    while(ftell(fp) != fim_csv){ 
        REGPARAMS aux;
        aux.removido = '0';
        aux.tamanhoRegistro = 25;
        aux.prox = -1;
        //Leitura dos campos de tamanho fixo
        if(fscanf(fp,"%d,",&aux.idAttack)!=1){
            aux.idAttack = -1;
        }
        if(fscanf(fp,"%d,",&aux.year)!=1){
            aux.year = -1;
        }
        if(fscanf(fp,"%f,",&aux.financialLoss)!=1){
            aux.financialLoss = -1;
        }

        int i = 0;
        char a; 
        while (fread(&a,sizeof(char),1,fp) == 1 && a != ',') {
            aux.country[i] = a;
            i++;
        }
        aux.country[i] = '\0';
        
        i=0;
        while(fread(&a,sizeof(char),1,fp) == 1 && a != ',') {
            aux.attackType[i] = a;
            i++;
        }
        aux.attackType[i] = '\0';

        i=0;
        while(fread(&a,sizeof(char),1,fp) == 1 && a!=',') {
            aux.targetIndustry[i] = a;
            i++;
        }
        aux.targetIndustry[i] = '\0';

        i=0;
        while(fread(&a,sizeof(char),1,fp) == 1 && a != '\n') {
            aux.defenseMechanism[i] = a;
            i++;
        }
        aux.defenseMechanism[i] = '\0';

        //Aumenta o tamanho do registro caso os campos variáveis existam
        if(aux.country[0]!='\0'){
            aux.tamanhoRegistro += strlen(aux.country)+2;
        }
        if(aux.attackType[0] != '\0'){
            aux.tamanhoRegistro += strlen(aux.attackType)+2;
        }
        if(aux.targetIndustry[0] != '\0'){
           aux.tamanhoRegistro += strlen(aux.targetIndustry)+2;
        }
        if(aux.defenseMechanism[0] != '\0'){
            aux.tamanhoRegistro += strlen(aux.defenseMechanism)+2;
        }

        REG_DADOS* reg = criar_regDados(aux);
        escrever_regDados(bin,reg);//escreve o registro criado no binário
        
        params_h.nroReqArq +=1;
        
        free(reg);
    }



    fseek(bin,0,SEEK_SET);
    HEADER* h = create_header(params_h);
    escrever_header(h,bin);
    free(h);
    fclose(fp);
    fclose(bin);

    return true;
}
