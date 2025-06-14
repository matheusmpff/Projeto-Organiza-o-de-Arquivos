#ifndef REMOVIDO_H
    #define REMOVIDO_H

    #include "RegRW.h"

    bool compara_campos(REG* reg, char* campos[], char* valores[], int tamanho);
    void remover_registro(FILE* bin,REG * reg, HEADER* h, int posReg);
    void remover_registros(char * nomebin, char* campos[], char* valores[], int tamanho);
    bool compara_campos(REG* reg, char* campos[], char* valores[], int tamanho);

#endif