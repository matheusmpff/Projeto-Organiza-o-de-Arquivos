#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "RegPrint.h"
#include "RegBusca.h"
#include "csvRead.h"



int main(){
    FILE *fp = fopen("teste.csv","w");
    fwrite("oi,oi,oi,oi",sizeof(char),9,fp);
    fclose(fp);

    fp = fopen("teste.csv","r");
    char str1[20],str2[20],str3[20],str4[20];
    fscanf(fp,"%[^,],%[^,],%[^,],%[^\n\r]",str1,str2,str3,str4);

    printf("%s\n%s\n%s\n%s\n",str1,str2,str3,str4);
    fclose(fp);
}