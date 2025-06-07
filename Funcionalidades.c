#include "RegBusca.h"
#include "RegPrint.h"
#include "RegRW.h"
#include "csvRead.h"
#include <stdlib.h>
#include <stdio.h>
#include "Funcionalidades.h"
#include "funcao-fornecida.c"

/* função para executar a funcionalidade 1 */
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

bool func3(char* string1, int n){
    return true;
}

/*Função para ler as entradas do programa e identificar qual 
funcionalidade deve ser chamada por meio da análise da primeira entrada.
 AS funcionalidades 1 e 3 precisam ler entradas a mais que a funcionalidade 2
logo usa-se condicionais para tratar essa situação
*/
void ler_entradas(){
    int func;
    char string1[20];
    char string2[20];
    int n;

    scanf("%d",&func);
    scanf("%s",string1);
    
    if(func == 1){
        scanf("%s",string2);
        func1(string1,string2);
    }
    if(func == 3){
        scanf("%d", &n);
        func3(string1,n);
    }
    if(func == 2){
        func2(string1);
    }
    
}