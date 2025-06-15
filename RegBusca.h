#ifndef RegBusca
    #define RegBusca
    #include <stdbool.h>
    #include <stdio.h>

    void busca_registro(char* nomeArquivo, char* campos[], char* valores[], int quantidade);
    bool comparCampos(REG* reg, char* campos[], char* valores[], int tamanho);

#endif