/**
 * main.c — versão funcional
 *
 * Usa funções de tabuleiro.c e logica.c sem duplicar nada.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tabuleiro.h"
#include "logica.h"

#define PONTOS_NORMAL 10
#define PONTOS_DIFICIL 15
#define VIDAS_INICIAIS 2

// HUD
static int pontos = 0;
static int vidas = VIDAS_INICIAIS;

// Desenha HUD na tela
static void desenhar_hud(Tabuleiro *tab) {
    int hudY = MINY + tab->linhas + 2;
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(MINX, hudY);
    printf("Pontos: %d  Vidas: %d", pontos, vidas);
    screenSetColor(WHITE, BLACK);
    screenUpdate();
}

int main() {
    srand((unsigned) time(NULL));
    screenInit(1);
    keyboardInit();
    timerInit(50);

    // posição inicial segura do jogador
    jogadorX = 4;
    jogadorY = 4;
    next_moveX = 0;
    next_moveY = 0;

    // carrega perguntas
    BancoPerguntas banco;
    if (!carregar_perguntas("data/perguntas.json", &banco)) {
        screenSetColor(RED, BLACK);
        screenGotoxy(1, MAXY + 2);
        printf("Aviso: nao foi possivel carregar data/perguntas.json");
        screenSetColor(WHITE, BLACK);
    }

    // criar tabuleiro
    Tabuleiro *tab = criar_tabuleiro(MAXY, MAXX);
    if (!tab) {
        screenSetColor(RED, BLACK);
        screenGotoxy(1, MAXY + 3);
        printf("Erro: falha ao criar tabuleiro.");
        screenSetColor(WHITE, BLACK);
        keyboardDestroy();
        screenDestroy();
        timerDestroy();
        return 1;
    }

    // popula tubarões iniciais
    int qtd = 12;
    for (int k = 0; k < qtd; k++) {
        int rx = rand() % (tab->colunas - 4) + 2;
        int ry = rand() % (tab->linhas - 4) + 2;
        if (rx == jogadorX && ry == jogadorY) { k--; continue; }
        tab->matriz[ry][rx] = 'S';
    }

    desenhar_tabuleiro(tab, jogadorX, jogadorY);
    desenhar_hud(tab);

    long frames = 0;

    while (1) {
        // duas perguntas normais
        printf("\nPergunta 1 da rodada:\n");
        int ok1 = perguntar_rodada(&banco, PONTOS_NORMAL);
        if (ok1) pontos += PONTOS_NORMAL;

        printf("\nPergunta 2 da rodada:\n");
        int ok2 = perguntar_rodada(&banco, PONTOS_NORMAL);
        if (ok2) pontos += PONTOS_NORMAL;

        // movimentação jogador
        printf("\nAgora mova o jogador (WASD). Pressione tecla: ");
        if (keyhit()) {
            char cc = readch();
            if (cc == 'w' || cc == 'W') { next_moveY=-1; next_moveX=0; }
            else if (cc == 's' || cc == 'S') { next_moveY=1; next_moveX=0; }
            else if (cc == 'a' || cc == 'A') { next_moveX=-1; next_moveY=0; }
            else if (cc == 'd' || cc == 'D') { next_moveX=1; next_moveY=0; }
            else if (cc == 'q' || cc == 'Q') { break; } // quit
            else { next_moveX = next_moveY = 0; }
        } else { next_moveX = next_moveY = 0; }

        // aplica movimento do jogador
        aplicar_movimento(tab);

        // tubarões perseguem
        mover_tubaroes_perseguicao(tab);

        // colisão com tubarão
        if (checar_colisao(tab)) {
            printf("\nUm tubarão te capturou! Responda a pergunta difícil para tentar escapar.\n");
            int ok = perguntar_tubarao(&banco, PONTOS_DIFICIL);
            if (ok) {
                pontos += PONTOS_DIFICIL;
                printf("Você escapou!\n");
                tab->matriz[jogadorY][jogadorX] = '.';
            } else {
                vidas--;
                printf("Você errou. Vidas restantes: %d\n", vidas);
                tab->matriz[jogadorY][jogadorX] = '.';
                if (vidas <= 0) {
                    printf("\n🦈 GAME OVER — Você foi pego pelo tubarão e não teve mais vidas.\n");
                    break;
                }
            }
        }

        desenhar_tabuleiro(tab, jogadorX, jogadorY);
        desenhar_hud(tab);
        frames++;
    }

    destruir_tabuleiro(tab);
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    printf("\nPontuação final: %d\n", pontos);
    return 0;
}
