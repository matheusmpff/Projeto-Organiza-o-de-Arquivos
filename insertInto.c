#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "Funcionalidades.h"

void escreverCabecalho(FILE *arquivo, HEADER *h) {
    fseek(arquivo, 0, SEEK_SET);
    int status = get_status(h);
    long int topo = get_topo(h);
    int proxByteOffset = get_proxByteOffset(h);
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


    fwrite(&status, sizeof(char), 1, arquivo);
    fwrite(&topo, sizeof(long int), 1, arquivo);
    fwrite(&proxByteOffset, sizeof(long int), 1, arquivo);
    fwrite(&nroReqArq, sizeof(int), 1, arquivo);
    fwrite(&nroReqRem, sizeof(int), 1, arquivo);
    fwrite(descreveident, sizeof(char), 24, arquivo);
    fwrite(descreveYear, sizeof(char), 28, arquivo);
    fwrite(descreveFl, sizeof(char), 29, arquivo);
    fwrite(&codDescreveCountry, sizeof(char), 1, arquivo);
    fwrite(descreveCountry, sizeof(char), 27, arquivo);
    fwrite(&codDescreveType, sizeof(char), 1, arquivo);
    fwrite(descreveType, sizeof(char), 39, arquivo);
    fwrite(&codDescreveTargetIndustry, sizeof(char), 1, arquivo);
    fwrite(descreveTarget, sizeof(char), 39, arquivo);
    fwrite(&codDescreveDefense, sizeof(char), 1, arquivo);
    fwrite(descreveDefense, sizeof(char), 68, arquivo);
}

long int encontrarEspacoFirstFit(FILE *arquivo, int tamanhoNecessario, HEADER *header) {
    if(get_topo(header) == -1)
        return -1; // Lista vazia

    long int atual = get_topo(header);
    long int anterior = -1;

    while(atual != -1) {
        fseek(arquivo, atual, SEEK_SET);

        // Le registro
        REG *registro = ler_registro(arquivo, header);

        // Verfica se registro esta removido
        if(get_removido(registro) != '1') {
            // Caso nao esteja, pula para o proximo
            atual = get_prox(registro);
            continue;
        }

        // First Fit: primeiro espaco que couber
        if(get_tamanhoRegistro(registro) >= tamanhoNecessario) {
            // Remove no da lista
            if(anterior == -1) {
                // primeiro no
                set_topo(header, get_prox(registro));
            } else {
                // atualiza o no anterior para pular este
                fseek(arquivo, anterior + sizeof(char) + sizeof(int), SEEK_SET);
                long int prox = get_prox(registro);
                fwrite(&prox, sizeof(long int), 1, arquivo);
            }

            set_nroReqRem(header, get_nroReqRem(header) - 1);
            return atual;
        }

        anterior = atual;
        atual = get_prox(registro);
    }

    return -1; // Nao encontrou espaco
}

void lerDadosEntrada(REG *registro) {
    char buffer[100];
    int bufferInt;

    // idAttack
    scanf("%d, ", &bufferInt);
    set_idAttack(registro, bufferInt);

    // year
    scanf("%s, ", buffer);
    if(strcmp(buffer, "NULO") == 0) {
        set_year(registro, -1);
    } else {
        set_year(registro, atoi(buffer));
    }

    // financialLoss
    scanf("%s, ", buffer);
    if(strcmp(buffer, "NULO") == 0) {
        set_financialLoss(registro, -1.0);
    } else {
        set_financialLoss(registro, atof(buffer));
    }

    // country
    scan_quote_string(buffer);
    set_country(registro, buffer);
    getchar();

    // attackType
    scan_quote_string(buffer);
    set_attackType(registro, buffer);
    getchar();

    // targetIndustry
    scan_quote_string(buffer);
    set_targetIndustry(registro, buffer);
    getchar();

    // defenseMechanism
    scan_quote_string(buffer);
    set_defenseMechanism(registro, buffer);
}

int calcularTamanhoRegistro(REG *registro) {
    int tamanho = 0;

    tamanho += sizeof(char); // removido
    tamanho += sizeof(int); // tamanhoRegistro
    tamanho += sizeof(long int); // proximo

    tamanho += sizeof(int); // idAttack
    tamanho += sizeof(int); // year
    tamanho += sizeof(float); // financialLoss

    if(strlen(get_country(registro)) > 0) {
        tamanho += sizeof(char) + strlen(get_country(registro)) + 1; // country

    }
    if(strlen(get_attackType(registro)) > 0) {
        tamanho += sizeof(char) + strlen(get_attackType(registro)) + 1; // country

    }
    if(strlen(get_targetIndustry(registro)) > 0) {
        tamanho += sizeof(char) + strlen(get_targetIndustry(registro)) + 1; // country

    }
    if(strlen(get_defenseMechanism(registro)) > 0) {
        tamanho += sizeof(char) + strlen(get_defenseMechanism(registro)) + 1; // country

    }
    
    return tamanho;
}

// Função para escrever um registro no arquivo (com preenchimento de lixo '$')
void escreverRegistro(FILE *arquivo, REG *registro, int tamanhoDisponivel, HEADER *header) {
    int tamanhoReal = calcularTamanhoRegistro(registro);
    
    escrever_registro(arquivo, registro, header);
    
    // PREENCHIMENTO COM '$' - Preencher espaço restante com lixo
    int espacoRestante = tamanhoDisponivel - tamanhoReal;
    for(int i = 0; i < espacoRestante; i++) {
        char lixo = '$';
        fwrite(&lixo, sizeof(char), 1, arquivo);
    }
}


void insertInto(char* arquivoBin, int numeroInsercoes) {
    FILE *fp = fopen(arquivoBin, "rb+");

    if(fp == NULL) {
        printf("Erro no processamento do arquivo\n");
        return;
    }
    
    HEADER* header = criar_header();
    ler_header(fp, header);

    // Seta arquivo como inconsistente
    set_status(header, 0);
    escreverCabecalho(fp, header);

    for(int i = 0; i < numeroInsercoes; i++) {
        REG *registro = criar_reg();
        lerDadosEntrada(registro);

        int tamanhoNecessario = calcularTamanhoRegistro(registro);

        long int posicaoInsercao = encontrarEspacoFirstFit(fp, tamanhoNecessario, header);

        if(posicaoInsercao != -1) {
            // Reutiliza o espaco do registro removido
            // Verifica o tamanho disponivel nessa posicao
            fseek(fp, posicaoInsercao, SEEK_SET);
            REG *registroremovido = ler_registro(fp, header);
            escreverRegistro(fp, registro, get_tamanhoRegistro(registroremovido), header);
            free(registroremovido);
        } else {
            // Insere no final do arquivo
            fseek(fp, 0, SEEK_END);
            escreverRegistro(fp, registro, tamanhoNecessario, header);
            set_proxByteOffset(header, get_proxByteOffset(header) + tamanhoNecessario);
        }

        set_nroReqArq(header, get_nroReqArq(header) + 1);

        free(registro);
    }

    // Marca arquivo como consistente
    set_status(header, 1);
    escreverCabecalho(fp, header);

    free(header);

    fclose(fp);
}