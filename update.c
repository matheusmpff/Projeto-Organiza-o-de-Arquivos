#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "remocao.h"
#include "insertInto.h"


void ajuste_reg(REG* reg,int tamanhoCampos,char*campos[],char* valores[]){
    int dif1 = 0;
    int dif2 = 0;
    int dif3 = 0;
    int dif4 = 0;
    for(int i = 0 ;i < tamanhoCampos; i++){
        

        if(campos[i] == NULL || valores[i] == NULL){
            continue;
        }
        if(strcmp(campos[i],"country") == 0){
            dif1 = strlen(valores[i]) - strlen(get_country(reg));
            set_country(reg,valores[i]);
            continue;
        }
        if(strcmp(campos[i],"attackType") == 0){
            dif2 = strlen(valores[i]) - strlen(get_attackType(reg));
            set_attackType(reg,valores[i]);
            continue;
        }
        if(strcmp(campos[i],"financialLoss") == 0){
            
            set_financialLoss(reg,atof(valores[i]));
            continue;
        }
        if(strcmp(campos[i],"defenseMechanism") == 0){
            dif3 = strlen(valores[i]) - strlen(get_defenseMechanism(reg));
            set_defenseMechanism(reg,valores[i]);
            continue;
        }
        if(strcmp(campos[i],"targetIndustry") == 0){
            dif4 = strlen(valores[i]) - strlen(get_targetIndustry(reg));
            set_targetIndustry(reg,valores[i]);
            continue;
        }
        
        if(strcmp(campos[i],"idAttack") == 0){
            set_idAttack(reg,atoi(valores[i]));
            continue;
        }
        if(strcmp(campos[i],"year") == 0){
            set_year(reg,atoi(valores[i]));
            continue;
        }
        
        
    }
    int tamanhoNovo = get_tamanhoRegistro(reg) + dif1 +dif2 + dif3 + dif4;
    set_tamanhoRegistro(reg,tamanhoNovo);
}


void atualizar_reg(char* nomebin,int tamanhoFiltro, char*camposF[] , char* valoresF[],int tamanhoCampos,char*campos[],char* valores[]){

    FILE* bin = fopen(nomebin,"rb+");

    if(bin == NULL){
        printf("Erro no processamento do arquivo\n");
        return;
    }
    fseek(bin,0,SEEK_END);
    int fim = ftell(bin);
    fseek(bin,0,SEEK_SET);

    HEADER* h = criar_header();
    ler_header(bin,h);

    int posReg = -1;
    REG* reg;

    while(ftell(bin) < fim){
        posReg = ftell(bin);

        reg = ler_registro(bin,h);
        if(compara_campos(reg, camposF, valoresF,tamanhoFiltro)){
            
            // if(tamanhoNovo<=get_tamanho_reg(reg)){
            //     fseek(bin,posReg,SEEK_SET);
            //     escrever_registro(bin,reg,h);
            //     adicionar_lixo(bin,tamanhoNovo - get_tamanho_reg(reg));
            // }
            // else{
                remover_registro(bin,reg,h,posReg);
                ajuste_reg(reg,tamanhoCampos,campos,valores);
                set_removido(reg,'0');
                set_prox(reg,-1);
                inserir_registro(bin,reg,h,get_tamanhoRegistro(reg));
            // }
        }
    }
    free(h);
}