#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/ranking.h"
#include "../include/screen.h"
#include "../include/keyboard.h"

// Armazena onde o arquivo foi encontrado ou criado com sucesso
static char caminho_final[64] = "data/ranking.json";

// Tenta abrir o arquivo em múltiplos locais possíveis baseados na estrutura do projeto
FILE* abrir_arquivo_smart(const char* modo) {
    FILE *f;
    
    // Array de tentativas de caminhos
    // 1. "data/..."   -> Executando da raiz do projeto
    // 2. "../data/.." -> Executando de dentro de 'build' ou 'src' ou 'bin'
    // 3. "../../data/." -> Executando de 'build/obj' (caso raro, mas possível)
    // 4. "ranking.json" -> Fallback: salva onde estiver o executável se não achar a pasta data
    const char* tentativas[] = {
        "data/ranking.json",
        "../data/ranking.json",
        "../../data/ranking.json",
        "ranking.json"
    };
    int num_tentativas = 4;

    // Se for leitura ("r"), tenta abrir e retorna o primeiro que existir
    if (strstr(modo, "r")) {
        for(int i=0; i<num_tentativas; i++) {
            f = fopen(tentativas[i], "r");
            if (f) {
                strcpy(caminho_final, tentativas[i]);
                return f;
            }
        }
        return NULL; // Nenhum arquivo existente encontrado
    }

    // Se for escrita ("w"), tenta abrir na ordem de preferência
    // O fopen "w" falha se o DIRETÓRIO não existir, então isso valida a pasta data
    for(int i=0; i<num_tentativas; i++) {
        f = fopen(tentativas[i], "w");
        if (f) {
            strcpy(caminho_final, tentativas[i]);
            return f;
        }
    }

    return NULL;
}

void ordenar_ranking(RegistroRanking lista[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            if (lista[j].pontos < lista[j + 1].pontos) {
                RegistroRanking temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }
}

void carregar_ranking(RegistroRanking lista[], int *qtd) {
    *qtd = 0;
    
    FILE *f = abrir_arquivo_smart("r");
    if (!f) return;

    char buffer[2048]; 
    int len = fread(buffer, 1, 2048, f);
    if (len <= 0) { fclose(f); return; } // Arquivo vazio ou erro
    buffer[len] = '\0';
    fclose(f);

    char *cursor = buffer;
    while (*qtd < MAX_RANKING) {
        char *inicio_nome = strstr(cursor, "\"nome\": \"");
        if (!inicio_nome) break;
        inicio_nome += 9;

        char *fim_nome = strchr(inicio_nome, '\"');
        if (!fim_nome) break;

        int tam_nome = fim_nome - inicio_nome;
        if (tam_nome > 10) tam_nome = 10;
        strncpy(lista[*qtd].nome, inicio_nome, tam_nome);
        lista[*qtd].nome[tam_nome] = '\0';

        char *inicio_pontos = strstr(fim_nome, "\"pontos\": ");
        if (!inicio_pontos) break;
        inicio_pontos += 10;

        lista[*qtd].pontos = atoi(inicio_pontos);

        *qtd = *qtd + 1;
        cursor = inicio_pontos;
    }
    ordenar_ranking(lista, *qtd);
}

void salvar_ranking(RegistroRanking lista[], int qtd) {
    // Tenta usar o último caminho conhecido
    FILE *f = fopen(caminho_final, "w");
    
    // Se falhar (ex: diretório mudou ou caminho_final era inválido), busca novamente
    if (!f) {
        f = abrir_arquivo_smart("w");
    }
    
    if (!f) return; // Não foi possível salvar em lugar nenhum

    fprintf(f, "[\n");
    for (int i = 0; i < qtd; i++) {
        fprintf(f, "  {\n");
        fprintf(f, "    \"nome\": \"%s\",\n", lista[i].nome);
        fprintf(f, "    \"pontos\": %d\n", lista[i].pontos);
        fprintf(f, "  }%s\n", (i < qtd - 1) ? "," : "");
    }
    fprintf(f, "]\n");
    fclose(f);
}

void adicionar_pontuacao(const char* nome, int pontos) {
    RegistroRanking lista[MAX_RANKING + 1];
    int qtd = 0;

    carregar_ranking(lista, &qtd);

    strncpy(lista[qtd].nome, nome, 10);
    lista[qtd].nome[10] = '\0';
    lista[qtd].pontos = pontos;
    qtd++;

    ordenar_ranking(lista, qtd);

    if (qtd > MAX_RANKING) qtd = MAX_RANKING;

    salvar_ranking(lista, qtd);
}

void desenhar_tela_ranking() {
    RegistroRanking lista[MAX_RANKING];
    int qtd = 0;
    carregar_ranking(lista, &qtd);

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MINX + 21, MINY + 12);
    printf("POS   NOME        PONTOS");
    
    screenGotoxy(MINX + 21, MINY + 13);
    printf("---   ----------  ------");

    screenSetColor(WHITE, BLACK);
    for (int i = 0; i < MAX_RANKING; i++) {
        screenGotoxy(MINX + 21, MINY + 14 + i);
        if (i < qtd) {
            printf("#%d    %-10s  %d", i + 1, lista[i].nome, lista[i].pontos);
        } else {
            printf("#%d    ----------  ---", i + 1);
        }
    }
}