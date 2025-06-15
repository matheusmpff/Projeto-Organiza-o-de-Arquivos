#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "RegRW.h"

bool compara_campos(REG* reg, char* campos[], char* valores[], int tamanho){
    if(get_removido(reg) == '1'){
        return false;
    }

    for(int i = 0 ;i < tamanho; i++){
        

        if(campos[i] == NULL || valores[i] == NULL){
            continue;
        }
        if(strcmp(campos[i],"country") == 0){
            if(strcmp(valores[i], get_country(reg)) != 0){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"attackType") == 0){
            if(strcmp(valores[i], get_attackType(reg)) != 0){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"financialLoss") == 0){
            float financialLoss = atof(valores[i]);
			float arredondado = roundf(get_financialLoss(reg) *100.0f) / 100.0f;
            if(financialLoss != arredondado){
                return false;
                
            }
            continue;
        }
        if(strcmp(campos[i],"defenseMechanism") == 0){
            if(strcmp(valores[i], get_defenseMechanism(reg)) != 0){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"targetIndustry") == 0){
            if(strcmp(valores[i], get_targetIndustry(reg)) != 0){
                return false;
            }
            
            continue;
        }
        
        if(strcmp(campos[i],"idAttack") == 0){
            if(atoi(valores[i]) != get_idAttack(reg)){
                return false;
            }
            continue;
        }
        if(strcmp(campos[i],"year") == 0){
            if(atoi(valores[i]) != get_year(reg)){
                return false;
            }
            continue;
        }
        
        
    }

    return true;
}

void remover_registro(FILE* bin,REG * reg, HEADER* h, int posReg){
    set_removido(reg,'1');
    set_nroReqRem(h,get_nroReqRem(h)+1);
    set_nroReqArq(h,get_nroReqArq(h)-1);
            
    //adicionar parte de mudar o topo
    set_prox(reg,get_topo(h));
    set_topo(h,posReg);

    fseek(bin,posReg,SEEK_SET);
    escrever_registro(bin,reg,h);

    int posFinal = ftell(bin);

    fseek(bin,0,SEEK_SET);
    escrever_header(bin,h);

    fseek(bin,posFinal,SEEK_SET);

    fflush(bin);

}

void remover_registros(char * nomebin, char* campos[], char* valores[], int tamanho){
    FILE * bin = fopen(nomebin,"rb+");
    if(bin == NULL){
        printf("Erro no processamento do arquivo\n");
        return;
    }

    
    fseek(bin,0,SEEK_END);//encontra o fim do arquivo para o loop
    long int fim = ftell(bin);
    fseek(bin,0,SEEK_SET);//COLOCA O PONTEIRO NO INICIO DO HEADER
    REG* reg;
    long int posReg = -1;
    HEADER * h = criar_header();
    ler_header(bin,h);

    

    while(ftell(bin) < fim){
        posReg = ftell(bin);
        
        
        reg = ler_registro(bin,h);
        if(compara_campos(reg,campos,valores,tamanho)){
            remover_registro(bin,reg,h,posReg);
        }
        

        free(reg);
    }
    fseek(bin,0,SEEK_SET);
    escrever_header(bin,h);
    free(h);
    fclose(bin);

    

}