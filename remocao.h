#ifndef REMOVIDO_H
    #define REMOVIDO_H

    #include "RegRW.h"

    void remover_registros(char * nomebin, char* campos[], char* valores[], int tamanho);
    bool compara_campos(REG* reg, char* campos[], char* valores[], int tamanho);

#endif