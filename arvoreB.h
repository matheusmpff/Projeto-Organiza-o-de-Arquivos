#ifndef ARVOREB_H
    #define ARVOREB_H
    #include <stdbool.h>
    
    bool criar_indice(char* nomeBin, char* nomeIndice);
    bool busca_arvore(char* nomebin, char* nomeindice,char* campos[],char*valores[],int tamanho,int id);
    void ajuste_indice_inserInto(char* nomebin,char* nomeindice,long int *valores,int tamanho);
    void executar_update(char* nomeBin, char* nomeIndice, int nUpdates);
#endif