#include <stdlib.h>
#include <stdio.h>
#include "tabuleiro.h"
#include "screen.h"

// Posição onde o tabuleiro aparece na tela
#define OFFSET_X 5
#define OFFSET_Y 3

#define TAB_LARGURA 80
#define TAB_ALTURA 30

// ------------------------------------------------------
// CRIAÇÃO DO TABULEIRO GIGANTE
// ------------------------------------------------------
Tabuleiro* criar_tabuleiro_gigante() {

    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    tab->linhas = TAB_ALTURA;
    tab->colunas = TAB_LARGURA;

    tab->matriz = (char**)malloc(tab->linhas * sizeof(char*));

    for (int i = 0; i < tab->linhas; i++) {
        tab->matriz[i] = (char*)malloc(tab->colunas * sizeof(char));
        for (int j = 0; j < tab->colunas; j++) {
            tab->matriz[i][j] = '.'; // vazio
        }
    }

    return tab;
}

// ------------------------------------------------------
// DESTRUIR TABULEIRO
// ------------------------------------------------------
void destruir_tabuleiro(Tabuleiro *tab) {
    for (int i = 0; i < tab->linhas; i++) {
        free(tab->matriz[i]);
    }
    free(tab->matriz);
    free(tab);
}

// ------------------------------------------------------
// DESENHO DO TABULEIRO GIGANTE
// ------------------------------------------------------
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {

    // Corrige posição do jogador
    if (jogadorX < 0) jogadorX = 0;
    if (jogadorX >= tab->colunas) jogadorX = tab->colunas - 1;
    if (jogadorY < 0) jogadorY = 0;
    if (jogadorY >= tab->linhas) jogadorY = tab->linhas - 1;

    // Desenho linha por linha
    for (int y = 0; y < tab->linhas; y++) {
        for (int x = 0; x < tab->colunas; x++) {

            screenGotoxy(OFFSET_X + x, OFFSET_Y + y);

            // Desenhar bordas
            int eh_borda =
                y == 0 ||
                y == tab->linhas - 1 ||
                x == 0 ||
                x == tab->colunas - 1;

            if (eh_borda) {
                screenSetColor(LIGHTGRAY, BLACK);
                printf("#");
                continue;
            }

            // Jogador
            if (x == jogadorX && y == jogadorY) {
                screenSetColor(RED, BLACK);
                printf("P");
                continue;
            }

            // Oceano padrão
            screenSetColor(CYAN, BLACK);
            printf("%c", tab->matriz[y][x]);
        }
    }

    screenSetColor(WHITE, BLACK);
}
