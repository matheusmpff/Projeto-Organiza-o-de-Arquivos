#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "Funcionalidades.h"

/*
    Implementação da funcionalidade 5, INSERT INTO, utilizando a estrategia First Fit
*/

/*
    Escreve o cabeçalho do arquivo binário na posição inicial (offset 0)
 
    O cabeçalho contém:
    - status: '0' = inconsistente, '1' = consistente
    - topo: ponteiro para o primeiro registro removido (-1 se lista vazia)
    - proxByteOffset: proxima posicao livre no arquivo
    - nroReqArq: numero de registros ativos no arquivo
    - nroReqRem: numero de registros removidos
    - campos de descrição: strings fixas que descrevem os campos dos registros

    @param arquivo Ponteiro para o arquivo binário aberto
    @param h Ponteiro para a estrutura HEADER com os dados do cabeçalho
*/
void escreverCabecalho(FILE *arquivo, HEADER *h) {
    // Posiciona no início do arquivo para escrever o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    
    // Extrai todos os campos da estrutura HEADER usando funções getter
    char status = get_status(h);
    long int topo = get_topo(h);
    long int proxByteOffset = get_proxByteOffset(h);
    int nroReqArq = get_nroReqArq(h);
    int nroReqRem = get_nroReqRem(h);
    char *descreveident = get_descreveident(h);
    char *descreveYear = get_descreveYear(h);
    char *descreveFl = get_descreveFl(h);
    char codDescreveCountry = get_codDescreveCountry(h);
    char *descreveCountry = get_descreveCountry(h);
    char codDescreveType = get_codDescreveType(h);
    char *descreveType = get_descreveType(h);
    char codDescreveTargetIndustry = get_codDescreveTargetIndustry(h);
    char *descreveTarget = get_descreveTarget(h);
    char codDescreveDefense = get_codDescreveDefense(h);
    char *descreveDefense = get_descreveDefense(h);

    // Escreve cada campo no arquivo binário com seu tamanho específico
    fwrite(&status, sizeof(char), 1, arquivo);                    // 1 byte
    fwrite(&topo, sizeof(long int), 1, arquivo);                  // 8 bytes
    fwrite(&proxByteOffset, sizeof(long int), 1, arquivo);        // 8 bytes
    fwrite(&nroReqArq, sizeof(int), 1, arquivo);                  // 4 bytes
    fwrite(&nroReqRem, sizeof(int), 1, arquivo);                  // 4 bytes
    fwrite(descreveident, sizeof(char), 23, arquivo);             // 23 bytes
    fwrite(descreveYear, sizeof(char), 27, arquivo);              // 27 bytes
    fwrite(descreveFl, sizeof(char), 28, arquivo);                // 28 bytes
    fwrite(&codDescreveCountry, sizeof(char), 1, arquivo);        // 1 byte
    fwrite(descreveCountry, sizeof(char), 26, arquivo);           // 26 bytes
    fwrite(&codDescreveType, sizeof(char), 1, arquivo);           // 1 byte
    fwrite(descreveType, sizeof(char), 38, arquivo);              // 38 bytes
    fwrite(&codDescreveTargetIndustry, sizeof(char), 1, arquivo); // 1 byte
    fwrite(descreveTarget, sizeof(char), 38, arquivo);            // 38 bytes
    fwrite(&codDescreveDefense, sizeof(char), 1, arquivo);        // 1 byte
    fwrite(descreveDefense, sizeof(char), 67, arquivo);           // 67 bytes
}

/*
    Implementa o algoritmo First Fit para encontrar o primeiro espaço adequado
    de um registro removido no arquivo binario que possa acomodar um novo registro.
 
    ALGORITMO FIRST FIT:
    1. Percorre o arquivo binario
    2. Para cada registro removido, verifica se o tamanho é suficiente
    3. Se encontrar um espaço adequado, remove-o da lista por meio de
        a. seta o topo para o registro seguinte
    4. Se não encontrar, retorna -1 (inserir no final do arquivo)

    @param arquivo Ponteiro para o arquivo binário
    @param tamanhoNecessario Tamanho em bytes necessário para o novo registro
    @param header Ponteiro para a estrutura HEADER
    @return Posição no arquivo onde inserir (-1 se não encontrou espaço adequado)
 */
long int encontrarEspacoFirstFit(FILE *arquivo, int tamanhoNecessario, HEADER *header) {
    if(get_topo(header) == -1)
        return -1; // Lista vazia

    long int atual = get_topo(header);
    long int anterior = -1;

    while(atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        
        // Le registro removido
        REG *registro = ler_registro(arquivo, header);
        
        // Verifica se registro está realmente removido
        if(get_removido(registro) != '1') {
            // Se não estiver removido, há um erro na lista
            long int prox = get_prox(registro);
            free(registro);
            anterior = atual;
            atual = prox;
            continue;
        }

        // First Fit: primeiro espaço que couber
        if(get_tamanhoRegistro(registro) >= tamanhoNecessario) {
            // Remove nó da lista
            if(anterior == -1) {
                // É o primeiro nó
                set_topo(header, get_prox(registro));
            } else {
                // Atualiza o nó anterior para pular este
                fseek(arquivo, anterior + sizeof(char) + sizeof(int), SEEK_SET);
                long int prox = get_prox(registro);
                fwrite(&prox, sizeof(long int), 1, arquivo);
            }

            // Decrementa contador de registros removidos
            set_nroReqRem(header, get_nroReqRem(header) - 1);
            
            long int posicaoRetorno = atual;
            free(registro);
            return posicaoRetorno;
        }

        anterior = atual;
        atual = get_prox(registro);
        free(registro);
    }

    return -1; // Não encontrou espaço adequado
}

void lerDadosEntrada(REG *registro) {
    char buffer[100];
    int bufferInt;

    // idAttack
    scanf("%d", &bufferInt);
    set_idAttack(registro, bufferInt);

    // year
    scanf("%s", buffer);
    if(strcmp(buffer, "NULO") == 0) {
        set_year(registro, -1);
    } else {
        set_year(registro, atoi(buffer));
    }

    // financialLoss
    scanf("%s", buffer);
    if(strcmp(buffer, "NULO") == 0) {
        set_financialLoss(registro, -1.0);
    } else {
        float valor;
        sscanf(buffer, "%f", &valor);
        set_financialLoss(registro, valor);
    }

    // country
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_country(registro, "-1000");
    } else {
        set_country(registro, buffer);
    }

    // attackType
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_attackType(registro, "-1000");
    } else {
        set_attackType(registro, buffer);
    }

    // targetIndustry
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_targetIndustry(registro, "-1000");
    } else {
        set_targetIndustry(registro, buffer);
    }

    // defenseMechanism
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_defenseMechanism(registro, "-1000");
    } else {
        set_defenseMechanism(registro, buffer);
    }
}

int calcularTamanhoRegistro(REG *registro) {
    int tamanho = 0;
    
    // Campos fixos do registro
    tamanho += sizeof(long int); // proximo  
    tamanho += sizeof(int);      // idAttack
    tamanho += sizeof(int);      // year
    tamanho += sizeof(float);    // financialLoss
    
    // Campos de string variáveis - seguindo o formato da escrever_registro
    
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


void inserir_registro(FILE* fp,REG* registro,HEADER* header, int tamanhoNecessario){
    long int posicaoInsercao = encontrarEspacoFirstFit(fp, tamanhoNecessario,header);
        
        if(posicaoInsercao != -1) {
            // Reutiliza espaço de registro removido
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            // Lê o tamanho do espaço disponível para calcular o lixo
            char removidoAntigo;
            int tamanhoDisponivel;
            fread(&removidoAntigo, sizeof(char), 1, fp);
            fread(&tamanhoDisponivel, sizeof(int), 1, fp);
            set_tamanhoRegistro(registro, tamanhoDisponivel);
            // Volta para o início do registro para escrever
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            // Escreve o registro usando a função corrigida
            escrever_registro(fp, registro, header);
            
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
            
            // Usa a função corrigida
            escrever_registro(fp, registro, header);
            
            set_proxByteOffset(header, ftell(fp));
        }

        set_nroReqArq(header, get_nroReqArq(header) + 1);
        free(registro);

        int posFinal = ftell(fp);
        escreverCabecalho(fp,header);
        fseek(fp,posFinal,SEEK_SET);
        fflush(fp);
}

void insertInto(char* arquivoBin, int numeroInsercoes) {
    FILE *fp = fopen(arquivoBin, "rb+");
    if(fp == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    HEADER* header = criar_header();
    ler_header(fp, header);

    // Seta arquivo como inconsistente
    set_status(header, '0');
    escreverCabecalho(fp,header);

    for(int i = 0; i < numeroInsercoes; i++) {
        REG *registro = criar_reg();
        lerDadosEntrada(registro);

        // Calcular tamanho e setar ANTES de procurar espaço
        int tamanhoCalculado = calcularTamanhoRegistro(registro);
        set_removido(registro, '0');
        set_prox(registro, -1);
        
        // Procura espaço disponível na lista de removidos
        long int posicaoInsercao = encontrarEspacoFirstFit(fp, tamanhoCalculado, header);
        
        if(posicaoInsercao != -1) {
            // Reutiliza espaço de registro removido
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            // Lê o tamanho do espaço disponível para calcular o lixo
            char removidoAntigo;
            int tamanhoDisponivel;
            fread(&removidoAntigo, sizeof(char), 1, fp);
            fread(&tamanhoDisponivel, sizeof(int), 1, fp);
            set_tamanhoRegistro(registro, tamanhoDisponivel);
            // Volta para o início do registro para escrever
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            escrever_registro(fp, registro, header);
            
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
            
            // Usa a função corrigida
            escrever_registro(fp, registro, header);
            
            set_proxByteOffset(header, ftell(fp));
        }

        set_nroReqArq(header, get_nroReqArq(header) + 1);
        free(registro);
    }

    // Marca arquivo como consistente
    set_status(header, '1');
    escreverCabecalho(fp, header);

    free(header);
    fclose(fp);
}