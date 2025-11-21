/**
  * Created on Aug, 23th 2023
  * Author: Tiago Barros
  * Based on "From C to C++ course - 2002"
  *
  * Observação: mantive todas as funções originais e apenas integrei
  * o tabuleiro com tubarões móveis e jogador emoji.
  */

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "tabuleiro.h" // ADICIONADO: integração com o tabuleiro

// Emoji do jogador
#define EMOJI_JOGADOR "🏄"

int x = 34, y = 12;
int incX = 1, incY = 1;

// Função para mover tubarões aleatoriamente
void mover_tubaroes(Tabuleiro *tab) {
    if (!tab) return;

    for (int i = 0; i < tab->linhas; i++) {
        for (int j = 0; j < tab->colunas; j++) {
            if (tab->matriz[i][j] == 'S') {
                // escolhe direção aleatória: 0=esq,1=dir,2=cima,3=baixo
                int dir = rand() % 4;
                int ni = i, nj = j;

                switch(dir) {
                    case 0: nj--; break;
                    case 1: nj++; break;
                    case 2: ni--; break;
                    case 3: ni++; break;
                }

                // verifica limites e se não sobrescreve o jogador
                if (ni > 1 && ni < tab->linhas-1 &&
                    nj > 1 && nj < tab->colunas-1 &&
                    !(ni == y && nj == x) &&
                    tab->matriz[ni][nj] != 'S') {

                    tab->matriz[ni][nj] = 'S';
                    tab->matriz[i][j] = '.';
                }
            }
        }
    }
}

void printHello(int nextX, int nextY)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(x, y);
    printf("           ");
    x = nextX;
    y = nextY;
    screenGotoxy(x, y);
    printf(EMOJI_JOGADOR);  // trocado para emoji surfista
}

void printKey(int ch)
{
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 22);
    printf("Key code :");

    screenGotoxy(34, 23);
    printf("            ");
    
    if (ch == 27) screenGotoxy(36, 23);
    else screenGotoxy(39, 23);

    printf("%d ", ch);
    while (keyhit())
    {
        printf("%d ", readch());
    }
}

int main() 
{
    static int ch = 0;
    static long timer = 0;

    // Inicializações originais
    screenInit(1);
    keyboardInit();
    timerInit(50);

    // -----------------------------
    // ADICIONADO: criar e popular tabuleiro
    // -----------------------------
    srand((unsigned) time(NULL));

    Tabuleiro *tab = criar_tabuleiro(MAXY, MAXX);
    if (tab == NULL) {
        screenSetColor(RED, BLACK);
        screenGotoxy(1, MAXY + 2);
        printf("Erro: falha ao criar tabuleiro. Continuando sem tabuleiro...");
        screenSetColor(WHITE, BLACK);
    } else {
        int qtd = 12;
        for (int k = 0; k < qtd; k++) {
            int rx = rand() % (tab->colunas - 4) + 2;
            int ry = rand() % (tab->linhas - 4) + 2;
            if (rx == x && ry == y) { k--; continue; }
            tab->matriz[ry][rx] = 'S';
        }

        desenhar_tabuleiro(tab, x, y);
    }
    // -----------------------------
    // Fim da parte ADICIONADA
    // -----------------------------

    printHello(x, y);
    screenUpdate();

    while (ch != 10 && timer <= 100)
    {
        if (keyhit()) 
        {
            ch = readch();
            printKey(ch);
            screenUpdate();
        }

        if (timerTimeOver() == 1)
        {
            int newX = x + incX;
            if (newX >= (MAXX -strlen(EMOJI_JOGADOR) -1) || newX <= MINX+1) incX = -incX;
            int newY = y + incY;
            if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;

            // -----------------------------
            // ADICIONADO: move tubarões a cada rodada
            // -----------------------------
            if (tab != NULL) {
                mover_tubaroes(tab);
                desenhar_tabuleiro(tab, newX, newY);
            }

            printHello(newX, newY);

            screenUpdate();
            timer++;
        }
    }

    if (tab != NULL) {
        destruir_tabuleiro(tab);
        tab = NULL;
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
