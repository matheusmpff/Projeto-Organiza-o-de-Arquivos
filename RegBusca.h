#ifndef RegBusca
    #define RegBusca
    #include <stdbool.h>
    typedef struct filtros FILTROS;

    FILTROS *criarFiltro(char *parametro, char *valor);
    void busca_registrador(FILE *fp, FILTROS *filtros, int quantidadeFiltros, int quantidadeBuscas);
    void set_filtro(FILTROS *filtros, char *parametro, char *valor, int posicaoDoFiltro);
    void ler_linha_busca(FILE *fp);



#endif