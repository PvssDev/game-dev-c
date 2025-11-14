
/**
 * tabuleiro.c
 * Criação e manipulação do tabuleiro do SharkLog
 * Observação: ainda está sem os tubarões
 */

#include <stdlib.h>
#include <stdio.h>
#include "tabuleiro.h"
#include "screen.h"

Tabuleiro* criar_tabuleiro(int linhas, int colunas) {
    Tabuleiro *tab = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    tab->linhas = linhas;
    tab->colunas = colunas;

    tab->matriz = (char**)malloc(linhas * sizeof(char*));
    for (int i = 0; i < linhas; i++) {
        tab->matriz[i] = (char*)malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) {
            tab->matriz[i][j] = '.'; // célula vazia
        }
    }

    return tab;
}

void destruir_tabuleiro(Tabuleiro *tab) {
    for (int i = 0; i < tab->linhas; i++) {
        free(tab->matriz[i]);
    }
    free(tab->matriz);
    free(tab);
}

void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY) {
    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            screenGotoxy(j + MINX, i + MINY);

            if (i == jogadorY && j == jogadorX) {
                screenSetColor(RED, BLACK);
                printf("P"); // jogador
            } else {
                screenSetColor(LIGHTBLUE, BLACK);
                printf("%c", tab->matriz[i][j]);
            }
        }
    }
    screenSetColor(WHITE, BLACK); // reset cor
}
