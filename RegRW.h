#ifndef RegRW
    #define RegRW
    #include <stdbool.h>
    typedef struct header HEADER;
    typedef struct reg_dados REG_DADOS;

    typedef struct regParam{
        char removido;
        int tamanhoRegistro;
        long int prox;
        int idAttack;
        int year;
        float financialLoss;
        char country[20];
        char attackType[20];
        char targetIndustry[20];
        char defenseMechanism[20];
    
    }REGPARAMS;

    HEADER create_header();
    REG_DADOS* criar_regDados();
    REG_DADOS* ler_regDados(FILE *fp);
    bool escrever_regDados(REG_DADOS* reg);
    void printt_reg(REG_DADOS* reg);

#endif
