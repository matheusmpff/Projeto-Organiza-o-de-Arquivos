#include "RegBusca.h"
#include "RegPrint.h"
#include "RegRW.h"
#include "csvRead.h"
#include <stdlib.h>
#include <stdio.h>

bool func1(char* string1, char*string2){
    if (csv_to_bin()){
        printf("Falha no processamento do arquivo.\n");
        return false;
    }
    binarioNaTela();
    return true;
    

}

bool func2(char * string1){
    FILE* fp = fopen("arquivoB.bin","rb");
    if(fp == NULL){
        printf("Falha no processamento do arquivo.\n");
        return false;
    }
    print_registros(fp);
}

ler_entradas(){
    int func;
    char string1[20];
    char string2[20];
    int n;

    scanf("%d",&func);
    scanf("%s",string1);
    
    if(func == 1){
        scanf("%s",string2);
    }
    if(func == 3){
        scanf("%s", &n);
    }
}