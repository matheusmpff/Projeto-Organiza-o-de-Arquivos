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

    typedef struct headerParam{
        char status;
        long int topo;
        long int proxByteOffset;
        int nroReqArq;
        int nroReqRem;
        char descreveIdentificador [24];
        char descreveYear[28];
        char descreverFinancialLoss[29];
        char codDescreveCountry;
        char descreveCountry[27];
        char codDescreveType;
        char descreveType[39];
        char codDescreveTargetIndustry;
        char descreveTargetIndustry[39];
        char codDescreveDefense;
        char descreveDefense[68];
    }HEADERPARAMS;

    HEADER* create_header();
    bool escrever_header(HEADER* h, FILE* fp);
    REG_DADOS* criar_regDados();
    REG_DADOS* ler_regDados(FILE *fp);
    bool escrever_regDados(FILE* fp, REG_DADOS* reg);
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
