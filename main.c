#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegPrint.h"
#include "RegBusca.h"
#include "csvRead.h"



int main(){
    csv_to_bin();
    FILE *fp = fopen("arquivoB.bin","rb");
    print_registros(fp);
    //ler_linha_busca(fp);
    fclose(fp);
}