#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "RegRW.h"
#include "Funcionalidades.h"

void escreverCabecalho(FILE *arquivo, HEADER *h) {
    fseek(arquivo, 0, SEEK_SET);
    char status = '0' + get_status(h);
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

void escrever_registro_corrigido(FILE* bin, REG* reg, HEADER* h) {
    char removido = get_removido(reg);
    int tamanho = get_tamanhoRegistro(reg);
    long int prox = get_prox(reg);
    int idAttack = get_idAttack(reg);
    int year = get_year(reg);
    float financialLoss = get_financialLoss(reg);
    
    fwrite(&removido, sizeof(char), 1, bin);
    fwrite(&tamanho, sizeof(int), 1, bin);
    fwrite(&prox, sizeof(long int), 1, bin);
    fwrite(&idAttack, sizeof(int), 1, bin);
    fwrite(&year, sizeof(int), 1, bin);
    fwrite(&financialLoss, sizeof(float), 1, bin);
    
    char* country = get_country(reg);
    if(strcmp(country, "-1") != 0) {
        fwrite("1", sizeof(char), 1, bin);
        fwrite(country, strlen(country), 1, bin);
        fwrite("|", sizeof(char), 1, bin);
    }
    
    char* attackType = get_attackType(reg);
    if(strcmp(attackType, "-1") != 0) {
        fwrite("2", sizeof(char), 1, bin);
        fwrite(attackType, strlen(attackType), 1, bin);
        fwrite("|", sizeof(char), 1, bin);
    }
    
    char* targetIndustry = get_targetIndustry(reg);
    if(strcmp(targetIndustry, "-1") != 0) {
        fwrite("3", sizeof(char), 1, bin);
        fwrite(targetIndustry, strlen(targetIndustry), 1, bin);
        fwrite("|", sizeof(char), 1, bin);
    }
    
    char* defenseMechanism = get_defenseMechanism(reg);
    if(strcmp(defenseMechanism, "-1") != 0) {
        fwrite("4", sizeof(char), 1, bin);
        fwrite(defenseMechanism, strlen(defenseMechanism), 1, bin);
        fwrite("|", sizeof(char), 1, bin);
    }
}

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
        set_country(registro, "-1");
    } else {
        set_country(registro, buffer);
    }

    // attackType
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_attackType(registro, "-1");
    } else {
        set_attackType(registro, buffer);
    }

    // targetIndustry
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_targetIndustry(registro, "-1");
    } else {
        set_targetIndustry(registro, buffer);
    }

    // defenseMechanism
    scan_quote_string(buffer);
    if(strlen(buffer) == 0) {
        set_defenseMechanism(registro, "-1");
    } else {
        set_defenseMechanism(registro, buffer);
    }
}

int calcularTamanhoRegistro(REG *registro) {
    int tamanho = 0;
    
    // Campos fixos do registro
    tamanho += sizeof(char);     // removido
    tamanho += sizeof(int);      // tamanhoRegistro
    tamanho += sizeof(long int); // proximo  
    tamanho += sizeof(int);      // idAttack
    tamanho += sizeof(int);      // year
    tamanho += sizeof(float);    // financialLoss
    
    // Campos de string variáveis - seguindo o formato da escrever_registro
    
    // country: se não for nulo, escreve "1" + string + "|"
    if(get_country(registro) != NULL && strcmp(get_country(registro), "-1") != 0) {
        tamanho += sizeof(char);                    // código "1"
        tamanho += strlen(get_country(registro));   // string
        tamanho += sizeof(char);                    // separador "|"
    }

    // attackType: se não for nulo, escreve "2" + string + "|"
    if(get_attackType(registro) != NULL && strcmp(get_attackType(registro), "-1") != 0) {
        tamanho += sizeof(char);                      // código "2"
        tamanho += strlen(get_attackType(registro));  // string
        tamanho += sizeof(char);                      // separador "|"
    }

    // targetIndustry: se não for nulo, escreve "3" + string + "|"
    if(get_targetIndustry(registro) != NULL && strcmp(get_targetIndustry(registro), "-1") != 0) {
        tamanho += sizeof(char);                          // código "3"
        tamanho += strlen(get_targetIndustry(registro));  // string
        tamanho += sizeof(char);                          // separador "|"
    }

    // defenseMechanism: se não for nulo, escreve "4" + string + "|"
    if(get_defenseMechanism(registro) != NULL && strcmp(get_defenseMechanism(registro), "-1") != 0) {
        tamanho += sizeof(char);                             // código "4"
        tamanho += strlen(get_defenseMechanism(registro));   // string
        tamanho += sizeof(char);                             // separador "|"
    }
    
    return tamanho;
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
    escreverCabecalho(fp, header);

    for(int i = 0; i < numeroInsercoes; i++) {
        REG *registro = criar_reg();
        lerDadosEntrada(registro);

        // Calcular tamanho e setar ANTES de procurar espaço
        int tamanhoCalculado = calcularTamanhoRegistro(registro);
        set_tamanhoRegistro(registro, tamanhoCalculado);
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
            
            // Volta para o início do registro para escrever
            fseek(fp, posicaoInsercao, SEEK_SET);
            
            // Escreve o registro usando a função corrigida
            escrever_registro_corrigido(fp, registro, header);
            
            long int posicaoAtual = ftell(fp);
            long int espacoUsado = posicaoAtual - posicaoInsercao;
            long int espacoRestante = tamanhoDisponivel - espacoUsado;
            
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
            escrever_registro_corrigido(fp, registro, header);
            
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