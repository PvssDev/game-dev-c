/**
 * src/tabuleiro.c
 * Implementação das funções de gerenciamento do mapa.
 * Responsável por alocar memória, limpar e desenhar o jogo na tela.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tabuleiro.h"
#include "../include/screen.h"

// --- Configurações de Borda (ASCII) ---
// Definidas aqui para uso interno na função de desenho.
#define BORDA_CANTOS       "+"
#define BORDA_HORIZONTAL   "-"
#define BORDA_VERTICAL     "|"

/**
 * Aloca memória dinamicamente para o tabuleiro.
 *
 * Como funciona a alocação de uma matriz 2D dinâmica em C:
 * 1. Alocamos a estrutura 'Tabuleiro'.
 * 2. Alocamos um array de ponteiros (char**) para representar as linhas.
 * 3. Para cada linha, alocamos um array de caracteres (char*) para as colunas.
 *
 * @param linhas Altura do mapa.
 * @param colunas Largura do mapa.
 * @return Ponteiro para a estrutura Tabuleiro pronta para uso.
 */
Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    // 1. Aloca a estrutura principal
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (!tab) return NULL; // Falha na alocação

    tab->linhas = linhas;
    tab->colunas = colunas;

    // 2. Aloca o array de ponteiros para as linhas
    tab->matriz = (char**)malloc(linhas * sizeof(char*));
    if (tab->matriz == NULL) {
        free(tab); // Limpa se falhar
        return NULL;
    }
    
    // 3. Aloca cada linha individualmente e preenche com água
    for (int i = 0; i < linhas; i++) {
        tab->matriz[i] = (char*)malloc(colunas * sizeof(char));
        // Preenche a linha com o caractere de água ('.')
        for (int j = 0; j < colunas; j++) {
            tab->matriz[i][j] = '.'; 
        }
    }

    return tab;
}

/**
 * Libera a memória alocada para evitar vazamentos (Memory Leaks).
 * A liberação deve ser feita na ordem inversa da criação:
 * Conteúdo das linhas -> Array de linhas -> Estrutura principal.
 *
 * @param tab Ponteiro para o tabuleiro a ser destruído.
 */
void destruir_tabuleiro(Tabuleiro *tab) {
    if (!tab) return;
    
    // Libera cada linha de caracteres
    for (int i = 0; i < tab->linhas; i++) {
        free(tab->matriz[i]);
    }
    
    // Libera o array de ponteiros
    free(tab->matriz);
    
    // Libera a estrutura
    free(tab);
}

/**
 * Desenha o estado atual do jogo no terminal.
 *
 * Lógica de Renderização "Camadas" (Painter's Algorithm):
 * O loop percorre cada célula (x,y). Em cada posição, verificamos o que deve ser desenhado
 * com a seguinte prioridade:
 * 1. Jogador (Surfista) - Se ele estiver na posição, desenha e ignora o resto.
 * 2. Tubarão ('S') - Se houver um tubarão na matriz, desenha.
 * 3. Água ('.') - Se não for nada disso, desenha o fundo.
 *
 * Importante: Usamos ". " (ponto + espaço) para a água porque Emojis ocupam
 * visualmente 2 espaços de caractere. Isso mantém o mapa alinhado.
 *
 * @param tab O mapa com os dados dos tubarões.
 * @param jogadorX Posição atual do jogador.
 * @param jogadorY Posição atual do jogador.
 */
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {
    if (!tab) return;

    int L = tab->linhas;
    int C = tab->colunas;

    screenSetColor(WHITE, BLACK);

    // --- 1. Desenha a Borda Superior ---
    screenGotoxy(MINX, MINY);
    printf("%s", BORDA_CANTOS);
    // Desenha 2 traços horizontais por coluna para compensar a largura dos emojis
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL); 
    printf("%s", BORDA_CANTOS);

    // --- 2. Desenha o Corpo do Mapa ---
    for (int y = 0; y < L; y++) {
        int screenY = MINY + 1 + y; // Calcula linha real na tela

        // Desenha parede esquerda
        screenGotoxy(MINX, screenY);
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);

        // Loop das colunas (Conteúdo)
        for (int x = 0; x < C; x++) {

            // CAMADA 1: JOGADOR SURFISTA
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(WHITE, BLUE); // Fundo azul para destaque
                printf("%s", EMOJI_JOGADOR);
                screenSetColor(WHITE, BLACK); 
                continue; // Pula para a próxima célula
            }

            // CAMADA 2: TUBARÃO (S)
            if (tab->matriz[y][x] == 'S') {
                screenSetColor(WHITE, BLACK);
                printf("%s", EMOJI_TUBARAO); 
                continue;
            }

            // CAMADA 3: ÁGUA (.)
            screenSetColor(CYAN, BLACK); 
            printf(". "); // Ponto + Espaço para manter alinhamento
        }

        // Desenha parede direita
        screenSetColor(WHITE, BLACK);
        printf("%s", BORDA_VERTICAL);
    }

    // --- 3. Desenha a Borda Inferior ---
    screenGotoxy(MINX, MINY + L + 1);
    printf("%s", BORDA_CANTOS);
    for (int i = 0; i < C; i++) printf("%s%s", BORDA_HORIZONTAL, BORDA_HORIZONTAL);
    printf("%s", BORDA_CANTOS);

    // Força a atualização visual do terminal imediatamente
    screenUpdate();
}