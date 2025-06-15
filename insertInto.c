#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "Funcionalidades.h"
/*
    Implementa o algoritmo First Fit para encontrar o primeiro espaco adequado
    de um registro removido no arquivo binario que possa acomodar um novo registro.
 
    ALGORITMO FIRST FIT:
    1. Percorre o arquivo binario
    2. Para cada registro removido, verifica se o tamanho é suficiente
    3. Se encontrar um espaco adequado, remove-o da lista e retorna sua posicao
    4. Se nao encontrar, retorna -1 (inserir no final do arquivo)

    @param arquivo Ponteiro para o arquivo binario
    @param tamanhoNecessario Tamanho em bytes necessario para o novo registro
    @param header Ponteiro para a estrutura HEADER
    @return Posicao no arquivo onde inserir (-1 se nao encontrou espaco adequado)
 */
long int encontrarEspacoFirstFit(FILE *arquivo, int tamanhoNecessario, HEADER *header) {
    if(get_topo(header) == -1)
        return -1; // arquivo vazio, nao ha espaco para reutilizar

    long int atual = get_topo(header); // posicao do primeiro registro do arquivo
    long int anterior = -1; // posicao do registro anterior (usada na hora da remocao do registro)

    // Percorre o arquivo binario enquanto houver registros
    while(atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        
        // Le o registro da posicao atual
        REG *registro = ler_registro(arquivo, header);
        
        // Verifica se registro está realmente marcado com removido
        if(get_removido(registro) != '1') {
            // Se não estiver removido, pula para o proximo do arquivo binario
            long int prox = get_prox(registro);
            free(registro);
            anterior = atual;
            atual = prox;
            continue;
        }

        // First Fit: verifica se o espaco é suficiente para o novo registro
        if(get_tamanhoRegistro(registro) >= tamanhoNecessario) {
            // Remove o registro do arquivo
            if(anterior == -1) {
                // É o primeiro registro, atualiza o topo do cabecalho
                set_topo(header, get_prox(registro));
            } else {
                // Caso nao seja o primeiro do arquivo, atualiza o paramento long in prox, do registro anterior
                fseek(arquivo, anterior + sizeof(char) + sizeof(int), SEEK_SET);
                long int prox = get_prox(registro);
                fwrite(&prox, sizeof(long int), 1, arquivo);
            }

            // Decrementa contador de registros removidos
            set_nroReqRem(header, get_nroReqRem(header) - 1);
            
            long int posicaoRetorno = atual;
            free(registro);
            return posicaoRetorno; // retorna a posicao encontrada
        }

        // Se o espaco nao for suciente, continua a percorrer o arquivo binario
        anterior = atual;
        atual = get_prox(registro);
        free(registro);
    }

    return -1; // Não encontrou espaço adequado
}

/*
    Le os dados de entrada do usuario (stdin) e preenche uma estrutura REG
 
    FORMATO DE ENTRADA ESPERADO:
    - idAttack: inteiro    escreverCabecalho(fp,header);
    - year: inteiro ou "NULO"
    - financialLoss: float ou "NULO" 
    - country: string entre aspas ou "NULO"
    - attackType: string entre aspas ou "NULO"
    - targetIndustry: string entre aspas ou "NULO"
    - defenseMechanism: string entre aspas ou "NULO"
 
    TRATAMENTO DE VALORES NULOS:
    - Inteiros nulos: -1
    - Float nulo: -1.0
    - Strings nulas: "-1000"

    LEITURA DE STRINGS ENTRE ASPAS:
    - Utiliza a funcao fornecida pela professora void scan_quote_string(char *str)
  
    @param registro Ponteiro para a estrutura REG a ser preenchida
 */
void lerDadosEntrada(REG *registro) {
    char buffer[100];
    int bufferInt;

    // idAttack
    scanf("%d", &bufferInt);
    set_idAttack(registro, bufferInt);

    // year
    scanf("%s", buffer);
    if(strcmp(buffer, "NULO") == 0) {
        set_year(registro, -1); // valor padrao para NULO
    } else {
        set_year(registro, atoi(buffer));
    }

    // financialLoss
    scanf("%s", buffer);
    if(strcmp(buffer, "NULO") == 0) {
        set_financialLoss(registro, -1.0); // valor padrao para NULO
    } else {
        float valor;
        sscanf(buffer, "%f", &valor);
        set_financialLoss(registro, valor);
    }

    // country
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_country(registro, "-1000"); // valor padrao para NULO
    } else {
        set_country(registro, buffer);
    }

    // attackType
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_attackType(registro, "-1000"); // valor padrao para NULO
    } else {
        set_attackType(registro, buffer);
    }

    // targetIndustry
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_targetIndustry(registro, "-1000"); // valor padrao para NULO
    } else {
        set_targetIndustry(registro, buffer);
    }

    // defenseMechanism
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_defenseMechanism(registro, "-1000"); // valor padrao para NULO
    } else {
        set_defenseMechanism(registro, buffer);
    }
}

/*
    Calcula o tamanho total em bytes que um registro ocupara no arquivo

    ESTRUTURA DO REGISTRO NO ARQUIVO:
    - Campos fixos: proximo (8), idAttack (4), year (4), financialLoss (4) = 20 bytes
    - Campos variaveis: cada string nao-nula é armazenada como:
       [código_campo (1 byte)] + [conteúdo_string] + [separador '|' (1 byte)]

    CÓDIGOS DOS CAMPOS:
    - '1': country
    - '2': attackType  
    - '3': targetIndustry
    - '4': defenseMechanism
 
    @param ponterio para registro a ser calculado
    @return Tamanho total em bytes que o registro ocupara
 */
int calcularTamanhoRegistro(REG *registro) {
    int tamanho = 0;
    
    // Campos fixos do registro
    tamanho += sizeof(long int); // proximo  
    tamanho += sizeof(int);      // idAttack
    tamanho += sizeof(int);      // year
    tamanho += sizeof(float);    // financialLoss
    
    // Campos de string variáveis - so ocupam espaco caso nao sejam nulos
    
    // country: se não for nulo, escreve "1" + string + "|"
    if(get_country(registro) != NULL && strcmp(get_country(registro), "-1000") != 0) {
        tamanho += sizeof(char);                    // código "1"
        tamanho += strlen(get_country(registro));   // string
        tamanho += sizeof(char);                    // separador "|"
    }

    // attackType: se não for nulo, escreve "2" + string + "|"
    if(get_attackType(registro) != NULL && strcmp(get_attackType(registro), "-1000") != 0) {
        tamanho += sizeof(char);                      // código "2"
        tamanho += strlen(get_attackType(registro));  // string
        tamanho += sizeof(char);                      // separador "|"
    }

    // targetIndustry: se não for nulo, escreve "3" + string + "|"
    if(get_targetIndustry(registro) != NULL && strcmp(get_targetIndustry(registro), "-1000") != 0) {
        tamanho += sizeof(char);                          // código "3"
        tamanho += strlen(get_targetIndustry(registro));  // string
        tamanho += sizeof(char);                          // separador "|"
    }

    // defenseMechanism: se não for nulo, escreve "4" + string + "|"
    if(get_defenseMechanism(registro) != NULL && strcmp(get_defenseMechanism(registro), "-1000") != 0) {
        tamanho += sizeof(char);                             // código "4"
        tamanho += strlen(get_defenseMechanism(registro));   // string
        tamanho += sizeof(char);                             // separador "|"
    }
    
    return tamanho;
}

/*
    Insere um registro no arquivo usando estrategia de reutilizacao de espaco

    ESTRATEGIA DE INSERCAO:
    1. Tenta encontrar espaco adequado na lista de registros removidos (First Fit)
    2. Se encontrar: reutiliza o espaco, preenchendo o restante com lixo ('$')
    3. Se nao encontrar: insere no final do arquivo
  
    GERENCIAMENTO DE ESPACO:
    - Quando reutiliza espaco, pode sobrar bytes nao utilizados
    - Esses bytes sao preenchidos com caractere '$' (lixo)
    - Isso mantem a integridade da estrutura do arquivo
  
    @param fp Ponteiro para o arquivo binario
    @param registro Ponteiro para a estrutura REG a ser inserida
    @param header Ponteiro para a estrutura HEADER
    @param tamanhoNecessario Tamanho em bytes necessario para o registro
 */
void inserir_registro(FILE* fp,REG* registro,HEADER* header, int tamanhoNecessario){
    long int posicaoInsercao = encontrarEspacoFirstFit(fp, tamanhoNecessario,header);
        
        if(posicaoInsercao != -1) {
            // Reutiliza espaco de registro removido
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            // Le o tamanho do espaco disponivel para calcular o lixo
            char removidoAntigo;
            int tamanhoDisponivel;
            fread(&removidoAntigo, sizeof(char), 1, fp);
            fread(&tamanhoDisponivel, sizeof(int), 1, fp);
            set_tamanhoRegistro(registro, tamanhoDisponivel);
            // Volta para o inicio do registro para escrever
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            // Escreve o novo registro
            escrever_registro(fp, registro, header);
            
            // Calculo para o preenchimento do lixo
            long int posicaoAtual = ftell(fp);
            long int espacoUsado = posicaoAtual - posicaoInsercao;
            long int espacoRestante = tamanhoDisponivel + sizeof(char) /*char removido*/ + sizeof(int) /*int tamanho registro*/ - espacoUsado;
            
            // Garante que preenche exatamente o espaço que sobrou com $
            if(espacoRestante > 0) {
                char lixo = '$';
                for(long int j = 0; j < espacoRestante; j++) {
                    fwrite(&lixo, sizeof(char), 1, fp);
                }
            }
        } else {
            // Insere no final do arquivo
            long int posicaoFinal = get_proxByteOffset(header);
            fseek(fp, posicaoFinal, SEEK_SET);
            
            // Escreve registro no final
            escrever_registro(fp, registro, header);
            
            // Atualiza o parametro proximo byte offset do header para o proximo byte livre
            set_proxByteOffset(header, ftell(fp));
        }

        // Incremente contador de registro ativos
        set_nroReqArq(header, get_nroReqArq(header) + 1);
        free(registro);

        int posFinal = ftell(fp);
        escrever_header(fp,header);
        fseek(fp,posFinal,SEEK_SET);
        fflush(fp);
}

/*
    Funcao principal para insercao de multiplos registros no arquivo
 
    PROCESSO DE INSERCAO:
    1. Abre o arquivo em modo leitura/escrita binaria ("rb+")
    2. Le o cabecalho existente
    3. Marca arquivo como inconsistente durante a operacao
    4. Para cada registro a inserir:
      - Lê dados da entrada
      - Calcula tamanho necessario
      - Procura espaco reutilizavel ou insere no final
   5. Marca arquivo como consistente ao final
   
   CONTROLE DE CONSISTENCIA:
   - Status '0': arquivo inconsistente (operacao em andamento)
   - Status '1': arquivo consistente (operacao concluída com sucesso)
   - Isso protege contra corrupcao em caso de interrupcao da operacao
   
   @param arquivoBin Nome do arquivo binario
   @param numeroInsercoes Quantidade de registros a serem inseridos
 */
void insertInto(char* arquivoBin, int numeroInsercoes) {
    FILE *fp = fopen(arquivoBin, "rb+");
    if(fp == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // Carrega o cabecalho
    HEADER* header = criar_header();
    ler_header(fp, header);

    // Seta arquivo como inconsistente
    set_status(header, '0');
    fseek(fp, 0, SEEK_SET);
    escrever_header(fp, header);

    // Loop de insercao de registros de acordo com o numero de insercoes a serem feitas
    for(int i = 0; i < numeroInsercoes; i++) {
        // Cria novo registro para armazenar dados da entrada
        REG *registro = criar_reg();
        lerDadosEntrada(registro);

        // COnfigura campos de controle do registro
        int tamanhoCalculado = calcularTamanhoRegistro(registro);
        set_removido(registro, '0');    // Marca como nao removido
        set_prox(registro, -1);         // Nao aponta para proximo (nao esta na lista)
        
        // Procura espaço reutilizavel no arquivo binario
        long int posicaoInsercao = encontrarEspacoFirstFit(fp, tamanhoCalculado, header);
        
        // Caso tenha encontrado um espaco
        if(posicaoInsercao != -1) {
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            // Lê o tamanho do espaço disponível para calcular o lixo
            char removidoAntigo;
            int tamanhoDisponivel;
            fread(&removidoAntigo, sizeof(char), 1, fp);
            fread(&tamanhoDisponivel, sizeof(int), 1, fp);
            set_tamanhoRegistro(registro, tamanhoDisponivel);

            // Reposiciona para o espaco encontrado e escreve o novo registro
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            escrever_registro(fp, registro, header);
            
            // Calcula o preenchimento de lixo
            long int posicaoAtual = ftell(fp);
            long int espacoUsado = posicaoAtual - posicaoInsercao;
            long int espacoRestante = tamanhoDisponivel + sizeof(char) /*char removido*/ + sizeof(int) /*int tamanho registro*/ - espacoUsado;
            
            // Garante que preenche exatamente o espaço que sobrou
            if(espacoRestante > 0) {
                char lixo = '$';
                for(long int j = 0; j < espacoRestante; j++) {
                    fwrite(&lixo, sizeof(char), 1, fp);
                }
            }
        } else {
            // Insere no final do arquivo
            long int posicaoFinal = get_proxByteOffset(header);
            fseek(fp, posicaoFinal, SEEK_SET);
            
            escrever_registro(fp, registro, header);
            
            set_proxByteOffset(header, ftell(fp));
        }

        // Incremente o contador de registros ativos
        set_nroReqArq(header, get_nroReqArq(header) + 1);
        free(registro);
    }

    // Marca arquivo como consistente
    set_status(header, '1');
    escrever_header(fp, header);

    free(header);
    fclose(fp);
}