#ifndef RegRW
    #define RegRW
    #include <stdbool.h>
    typedef struct header HEADER;
    typedef struct reg REG;

    void escrever_cabecalho(FILE* fp, FILE* bin, HEADER* header);
    void escrever_registros(FILE* fp, FILE* bin, REG* reg);
    REG* criar_reg();
    void aux_ler_registro(FILE* bin, REG * reg, HEADER* h);
    REG* ler_registro(FILE* bin,HEADER *h);
    HEADER* criar_header();
    void ler_header(FILE* bin, HEADER* h);
    void printar_registro(REG* reg,HEADER* h);
    void printar_header(HEADER* h);
    void printar_binario(char * nome);
    char get_removido(REG *r);
    int get_idAttack(REG *r);
    int get_year(REG *r);
    float get_financialLoss(REG *r);
    char* get_country(REG *r);
    char* get_attackType(REG *r);
    char* get_targetIndustry(REG *r);
    char* get_defenseMechanism(REG *r);
    

#endif
