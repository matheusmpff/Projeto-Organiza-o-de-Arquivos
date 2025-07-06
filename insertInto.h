#ifndef INSERTINTO_H
    #define INSERTINTO_H

    #include "RegRW.h"

    void inserir_registro(FILE* fp,REG* registro,HEADER* header, int tamanhoNecessario);
    long int* insertInto(char* arquivoBin, int numeroInsercoes);
    
#endif 