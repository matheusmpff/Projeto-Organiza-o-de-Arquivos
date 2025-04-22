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
    char get_removido(REG_DADOS *r);
    int get_idAttack(REG_DADOS *r);
    int get_year(REG_DADOS *r);
    float get_financialLoss(REG_DADOS *r);
    char* get_country(REG_DADOS *r);
    char* get_attackType(REG_DADOS *r);
    char* get_targetIndustry(REG_DADOS *r);
    char* get_defenseMechanism(REG_DADOS *r);

#endif
