#ifndef RegPrint
    #define RegPrint
    #include <stdio.h>
    #include "RegRW.h"

    void print_registros(FILE *fp);
    int verificar_vazio(FILE *fp);
    void imprimir_registros(REG *r);
#endif