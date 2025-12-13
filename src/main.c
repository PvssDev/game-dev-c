/**
 * src/main.c
 * Versão Final - MENU
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/tabuleiro.h" 
#include "../include/jogador.h"
#include "../include/jogo.h"
#include "../include/ranking.h"

// Garante visibilidade das funções do jogo.c
void jogo_inicializar_tubaroes(Tabuleiro *tab, int pontuacao);
void jogo_resetar_tubaroes(Tabuleiro *tab, int pontuacao);
int verificar_colisao_lula(Jogador *j, Tabuleiro *tab);
void gerenciar_lula_ciclo(Tabuleiro *tab, Jogador *j);
void animar_punicao(Tabuleiro *tab, Jogador *j);
void lula_colisao_reset(Tabuleiro *tab, Jogador *j);

// --- FUNÇÕES DE MENU E INTERFACE ---

void desenhar_logo_sharklog() {
    screenSetColor(BLUE, BLACK);
    // Movemos o logo 10 casas para a direita para centralizar melhor
    screenGotoxy(MINX + 10, MINY + 2);
    printf("  ____  _                 _    _              ");
    screenGotoxy(MINX + 10, MINY + 3);
    printf(" / ___|| |__   __ _ _ __| | _| |    ___  __ _ ");
    screenGotoxy(MINX + 10, MINY + 4);
    printf(" \\___ \\| '_ \\ / _` | '__| |/ / |   / _ \\ / _` |");
    screenGotoxy(MINX + 10, MINY + 5);
    printf("  ___) | | | | (_| | |  |   <| |__| (_) | (_| |");
    screenGotoxy(MINX + 10, MINY + 6);
    printf(" |____/|_| |_|\\__,_|_|  |_|\\_\\_____\\___/ \\__, |");
    screenGotoxy(MINX + 10, MINY + 7);
    printf("                                         |___/ ");
}

// Função para capturar o nome do jogador
void input_nome_jogador(char *buffer) {
    screenClear();
    desenhar_logo_sharklog();

    screenSetColor(CYAN, BLACK);
    screenGotoxy(MINX + 15, MINY + 12);
    printf("DIGITE SEU NOME (Max 10 chars):");

    // Caixa de input
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX + 15, MINY + 14);
    printf("[          ]");
    
    int pos = 0;
    memset(buffer, 0, 11); // Limpa buffer

    while(1) {
        screenGotoxy(MINX + 16 + pos, MINY + 14); // Posiciona cursor visualmente
        
        if (keyhit()) {
            int ch = readch();
            
            // Enter (13 ou 10) confirma
            if (ch == 13 || ch == 10) {
                if (pos > 0) break; // Só sai se digitou algo
            }
            // Backspace (127 ou 8) apaga
            else if (ch == 127 || ch == 8) {
                if (pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                    screenGotoxy(MINX + 16 + pos, MINY + 14);
                    printf(" "); // Apaga visualmente
                }
            }
            // Caracteres alfanuméricos
            else if (isalnum(ch) || ch == ' ') {
                if (pos < 10) {
                    buffer[pos] = (char)ch;
                    printf("%c", ch);
                    pos++;
                    buffer[pos] = '\0';
                }
            }
        }
        usleep(10000);
    }
}

void tela_ranking() {
    screenClear();
    desenhar_logo_sharklog();
    
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MINX + 25, MINY + 10);
    printf("=== RANKING ===");
    
    // Chama a função do novo módulo ranking.c
    desenhar_tela_ranking();
    
    screenSetColor(GREEN, BLACK);
    screenGotoxy(MINX + 12, MINY + 22);
    printf("[Pressione qualquer tecla para voltar]");
    screenUpdate();
    
    // Limpa buffer e espera tecla
    while(keyhit()) readch();
    readch(); 
}

int tela_menu() {
    int opcao = 0; // 0: Jogar, 1: Ranking, 2: Sair
    
    while(1) {
        screenClear();
        desenhar_logo_sharklog();

        screenSetColor(WHITE, BLACK);
        
        // Opção JOGAR
        screenGotoxy(MINX + 26, MINY + 11);
        if (opcao == 0) {
            screenSetColor(GREEN, BLACK);
            printf("> JOGAR <");
        } else {
            screenSetColor(WHITE, BLACK);
            printf("  JOGAR  ");
        }

        // Opção RANKING
        screenGotoxy(MINX + 26, MINY + 13);
        if (opcao == 1) {
            screenSetColor(GREEN, BLACK);
            printf("> RANKING <");
        } else {
            screenSetColor(WHITE, BLACK);
            printf("  RANKING  ");
        }

        // Opção SAIR
        screenGotoxy(MINX + 26, MINY + 15);
        if (opcao == 2) {
            screenSetColor(GREEN, BLACK);
            printf("> SAIR <");
        } else {
            screenSetColor(WHITE, BLACK);
            printf("  SAIR  ");
        }
        
        screenGotoxy(MINX + 8, MINY + 18);
        screenSetColor(CYAN, BLACK);
        printf("Use [W/S] para selecionar e [ENTER] para confirmar");

        screenUpdate();

        if (keyhit()) {
            int ch = readch();
            if (ch == 'w' || ch == 'W') {
                opcao--;
                if (opcao < 0) opcao = 2;
            }
            if (ch == 's' || ch == 'S') {
                opcao++;
                if (opcao > 2) opcao = 0;
            }
            if (ch == 13 || ch == 10 || ch == ' ') { 
                return opcao;
            }
        }
        usleep(50000);
    }
}

// --- LÓGICA DO JOGO ENCAPSULADA ---
void rodar_partida(char *nome_jogador) {
    screenClear();
    
    Tabuleiro *tab = criar_tabuleiro(ALTURA_JOGO, LARGURA_JOGO);
    if (!tab) return;

    Jogador *surfista = criar_jogador(tab);
    if (!surfista) return;

    // Atribui o nome capturado ao jogador
    strncpy(surfista->nome, nome_jogador, 10);
    surfista->nome[10] = '\0';

    jogo_resetar_tubaroes(tab, surfista->pontuacao); 
    
    screenClear(); 

    int game_running = 1;

    while (game_running && surfista->vidas > 0) {
        
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        // Fase de Perguntas
        if (!jogo_fase_perguntas(tab, surfista)) {
            game_running = 0; 
            break;
        }

        jogo_inicializar_tubaroes(tab, surfista->pontuacao);

        screenClear(); 
        desenhar_HUD(surfista);
        desenhar_tabuleiro(tab, surfista->x, surfista->y);
        
        int Y_MSG = MINY + ALTURA_JOGO + 4;
        
        if (!game_running) break;
        screenGotoxy(MINX, Y_MSG);
        printf("                                    ");

        // --- 1. COLISÃO COM LULA (🦑) ---
        if (verificar_colisao_lula(surfista, tab)) {
             screenSetColor(MAGENTA, BLACK);
             screenGotoxy(MINX, Y_MSG);
             printf("LULA TE PEGOU! SEGURADO PELOS TENTACULOS!");
             
             screenUpdate();
             fflush(stdout); 
             usleep(1000 * 3000);

             animar_punicao(tab, surfista);
             lula_colisao_reset(tab, surfista);
             
             while(keyhit()) readch();
             screenGotoxy(MINX, Y_MSG);
             printf("                                    ");
        }

        // --- 2. COLISÃO COM TUBARÃO (S) ---
        if (verificar_colisao(surfista, tab)) {
            
            if (tab->matriz[surfista->y][surfista->x] == 'S') {
                tab->matriz[surfista->y][surfista->x] = '.';
            }
            
            surfista->vidas--;
            
            screenSetColor(RED, BLACK);
            screenGotoxy(MINX, Y_MSG);
            printf("AI! VOCE FOI MORDIDO! -1 VIDA");
            
            screenUpdate();
            fflush(stdout); 
            usleep(2000 * 1000); 
            
            while(keyhit()) readch();
            
            screenGotoxy(MINX, Y_MSG);
            printf("                     ");
            screenUpdate();
        }
    }

    // --- FIM DE JOGO ---
    screenClear();
    screenGotoxy(MAXX/2 - 10, MAXY/2);
    if (surfista->vidas <= 0) {
        screenSetColor(RED, BLACK);
        printf("GAME OVER, %s!", surfista->nome);
    } else {
        screenSetColor(GREEN, BLACK);
        printf("JOGO ENCERRADO, %s.", surfista->nome);
    }
    screenGotoxy(MAXX/2 - 10, MAXY/2 + 2);
    printf("Pontuação Final: %d", surfista->pontuacao);

    // Salva a pontuação no JSON!
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MAXX/2 - 10, MAXY/2 + 4);
    printf("Salvando ranking...");
    adicionar_pontuacao(surfista->nome, surfista->pontuacao);
    printf(" OK!");

    screenUpdate();
    usleep(3000 * 1000); 
    
    while(keyhit()) readch();

    // Limpeza da partida
    destruir_jogador(surfista);
    destruir_tabuleiro(tab);
}

// --- MAIN PRINCIPAL ---
int main() {
    screenInit(1);
    keyboardInit();
    timerInit(1000); 
    srand((unsigned) time(NULL));

    char nome_atual[11] = "Jogador";

    int sair = 0;
    while (!sair) {
        int opcao = tela_menu();
        
        switch (opcao) {
            case 0: // JOGAR
                input_nome_jogador(nome_atual); // Pede o nome antes de começar
                rodar_partida(nome_atual);
                break;
            case 1: // RANKING
                tela_ranking();
                break;
            case 2: // SAIR
                sair = 1;
                break;
        }
    }

    screenClear();
    screenGotoxy(MINX, MINY);
    printf("Obrigado por jogar SharkLog!");
    screenUpdate();
    usleep(1000 * 1000);

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}