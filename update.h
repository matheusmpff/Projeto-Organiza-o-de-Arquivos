#ifndef UPDATE_H
    #define UPDATE_H

    #include "RegRW.h"

    void atualizar_reg(char* nomebin,int tamanhoFiltro, char*camposF[] , char* valoresF[],int tamanhoCampos,char*campos[],char* valores[]);
    void insertInto(char* arquivoBin, int numeroInsercoes);
    
#endif