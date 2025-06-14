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
    int get_tamanhoRegistro(REG *r);
    void set_tamanhoRegistro(REG *r, int x);
    void set_idAttack(REG *r, int idAttack);
    void set_year(REG *r, int year);
    void set_financialLoss(REG *r, float financialLoss);
    void set_country(REG *r, char *country);
    void set_attackType(REG *r, char *attacktype);
    void set_targetIndustry(REG *r, char *targetIndustry);
    void set_defenseMechanism(REG *r, char *defenseMechanism);


    long int get_topo(HEADER *h);
    bool set_topo(HEADER *h, int x);
    bool set_nroReqRem(HEADER *h, int x);
    int get_nroReqRem(HEADER *h);
    bool set_nroReqArq(HEADER *h,int x);
    int get_nroReqArq(HEADER *h);
    int get_proxByteOffset(HEADER *h);
    void set_proxByteOffset(HEADER *h, long int x);
    char get_codDescreveCountry(HEADER *h);
    char get_codDescreveType(HEADER *h);
    char get_codDescreveTargetIndustry(HEADER *h);
    char get_codDescreveDefense(HEADER *h);
    char get_status(HEADER * h);
    bool set_status(HEADER * h,char x);
    char * get_descreveident(HEADER *h);
    bool set_descreveident(HEADER *h,char * src);
    char * get_descreveYear(HEADER *h);
    bool set_descreveYear(HEADER *h, char*src);
    char * get_descreveFl(HEADER * h);
    bool set_descreveFl(HEADER *h, char * src);
    char *get_descreveCountry(HEADER *h);
    bool set_descreveCountry(HEADER * h,char * src);
    char *get_descreveType(HEADER *h);
    bool set_descreveType(HEADER *h, char* src);
    char * get_descreveTarget(HEADER *h);
    bool set_descreveTarget(HEADER *h, char* src);
    char * get_descreveDefense(HEADER *h);
    bool set_descreveDefense(HEADER *h, char* src);
#endif
