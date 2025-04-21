#ifndef RegRW
    #define RegRW

    typedef struct header HEADER;
    typedef struct reg_dados REG_DADOS;

    HEADER create_header();
    REG_DADOS criar_regDados();
    REG_DADOS ler_regDados(FILE *fp);

#endif
