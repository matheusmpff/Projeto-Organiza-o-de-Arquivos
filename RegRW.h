#ifndef RegRW
    #define RegRW
    #include <stdbool.h>
    typedef struct header HEADER;
    typedef struct reg REG;

    void escrever_cabecalho(FILE* fp, FILE* bin, HEADER* header);
    void escrever_registro(FILE* bin, REG* reg, HEADER* h);
    void escrever_registrosCSV(FILE* fp, FILE* bin, REG* reg, HEADER* h);
    REG* criar_reg();
    void aux_ler_registro(FILE* bin, REG * reg, HEADER* h);
    REG* ler_registro(FILE* bin,HEADER *h);
    HEADER* criar_header();
    void ler_header(FILE* bin, HEADER* h);
    void printar_registro(REG* reg,HEADER* h);
    void printar_header(HEADER* h);
    void printar_binario(char * nome);

    long int get_prox(REG* reg);
    void set_prox(REG* reg,long int prox);
    char get_removido(REG *r);
    void set_removido(REG* reg,char valor);
    int get_idAttack(REG *r);
    int get_year(REG *r);
    float get_financialLoss(REG *r);
    char* get_country(REG *r);
    char* get_attackType(REG *r);
    char* get_targetIndustry(REG *r);
    char* get_defenseMechanism(REG *r);


    long int get_topo(HEADER *h);
    bool set_topo(HEADER *h, int x);
    bool set_nroReqRem(HEADER *h, int x);
    int get_nroReqRem(HEADER *h);
    bool set_nroReqArq(HEADER *h,int x);
    int get_nroReqArq(HEADER *h);
    int get_proxByteOffset(HEADER *h);
    void set_proxByteOffset(HEADER *h, long int x);

#endif
