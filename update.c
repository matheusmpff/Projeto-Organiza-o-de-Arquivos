#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "remocao.h"
#include "insertInto.h"

/*
Pârametros: 
    reg -> structs registro que será atualizada
    campos -> array com campos que devem ser atualizados
    valores -> array dos valores que devem ser alterados
    tamanhoCampos -> quantidade de campos que devem ser alterados

Função responsável por ajustar os campos que devem ser atualizados.
*/
void ajuste_reg(REG* reg,int tamanhoCampos,char*campos[],char* valores[]){
    int dif1 = 0;//variavel para calcular a diferença se o country mudar
    int dif2 = 0;//variavel para calcular a diferença se o attackType mudar
    int dif3 = 0;//variavel para calcular a diferença se o financialLoss mudar
    int dif4 = 0;//variavel para calcular a diferença se o defenseMechanism mudar
    for(int i = 0 ;i < tamanhoCampos; i++){

        if(campos[i] == NULL || valores[i] == NULL){
            continue;
        }
        
        if(strcmp(campos[i],"country") == 0){
            //verifica se o campo atual a ser removido é country se sim faz a mudança e calcula a diferença
            dif1 = strlen(valores[i]) - strlen(get_country(reg));
            set_country(reg,valores[i]);
            continue;
        }
        if(strcmp(campos[i],"attackType") == 0){
            //verifica se o campo atual a ser removido é attackType se sim faz a mudança e calcula a diferença
            dif2 = strlen(valores[i]) - strlen(get_attackType(reg));
            set_attackType(reg,valores[i]);
            continue;
        }
        if(strcmp(campos[i],"financialLoss") == 0){
            //verifica se o campo atual a ser removido é financialLoss, não tem diferença
            // pois sempre ocupa o mesmo espaço
            set_financialLoss(reg,atof(valores[i]));
            continue;
        }
        if(strcmp(campos[i],"defenseMechanism") == 0){
            //verifica se o campo atual a ser removido é defenseMechanism se sim faz a mudança e calcula a diferença
            dif3 = strlen(valores[i]) - strlen(get_defenseMechanism(reg));
            set_defenseMechanism(reg,valores[i]);
            continue;
        }
        if(strcmp(campos[i],"targetIndustry") == 0){
            //verifica se o campo atual a ser removido é targetIndustry se sim faz a mudança e calcula a diferença
            dif4 = strlen(valores[i]) - strlen(get_targetIndustry(reg));
            set_targetIndustry(reg,valores[i]);
            continue;
        }
        
        if(strcmp(campos[i],"idAttack") == 0){
            //verifica se o campo atual a ser removido é idAttack se sim faz a mudança
            set_idAttack(reg,atoi(valores[i]));
            continue;
        }
        if(strcmp(campos[i],"year") == 0){
            //verifica se o campo atual a ser removido é year se sim faz a mudança
            set_year(reg,atoi(valores[i]));
            continue;
        }
        
        
    }
    // calcula o novo tamanho do registro
    int tamanhoNovo = get_tamanhoRegistro(reg) + dif1 +dif2 + dif3 + dif4;
    set_tamanhoRegistro(reg,tamanhoNovo);
}

/*
parâmetros:
    nomebin -> nome do arquivo binário a ser atualizado
    tamanhoFiltro -> quantidade de campos que serão utilizados para filtrar o registro a ser atualizado
    camposF,valoresF -> campos e seus respectivos valores que devem ser utilizados como filtro
    tamanhoCampos -> quantidade de campos a serem atualizados
    campos,valores -> os campos e seus respectivos valores que serão os novos valores dos registros

Função responsável por fazer a leitura do header e dos registros para passar eles como parâmetro para as
 funções auxiliares de filtro e ajuste de registro, além de ser responsável por chamar as funções de remoção
  e inserção quando necessárias. A função sempre remove o registro mesmo havendo espaço nele pois ao 
  remove-lo ele vai para o topo da lista dos removidos e será selecionado para ser inserido. Assim
  cumpre-se a condição de utilizar mesmo registro se tiver espaço nele.

*/
void atualizar_reg(char* nomebin,int tamanhoFiltro, char*camposF[] , char* valoresF[],int tamanhoCampos,char*campos[],char* valores[]){

    FILE* bin = fopen(nomebin,"rb+");

    if(bin == NULL){
        printf("Erro no processamento do arquivo\n");
        return;
    }
    fseek(bin,0,SEEK_END);//vai para o fim do arquivo
    int fim = ftell(bin);//guarda a posição final do arquivo
    fseek(bin,0,SEEK_SET);//volta para o inicio do arquivo

    HEADER* h = criar_header();
    ler_header(bin,h);//lê o header do arquivo binário

    int posReg = -1;// variável que ira guardar a posição do registro
    REG* reg; // ponteiro para o registros que será retornado pela função ler_registro

    //loop para percorrer todo o arquivo para achar os registros que devem ser atualizados
    while(ftell(bin) < fim){
        posReg = ftell(bin);//guarda a posição do registro

        reg = ler_registro(bin,h);
        //compara para ver se o registro passa no filtro se sim executa a remoção ajuste e inserção
        if(compara_campos(reg, camposF, valoresF,tamanhoFiltro)){
            remover_registro(bin,reg,h,posReg);
            ajuste_reg(reg,tamanhoCampos,campos,valores);
            set_removido(reg,'0');//atualiza para indicar que não está removido
            set_prox(reg,-1);//como não está removido coloca-se -1 na posição do proximo removido
            inserir_registro(bin,reg,h,get_tamanhoRegistro(reg)); // insere o registro atualizado
        }
    }
    free(h);
}