#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include "../include/jogo.h"
#include "../include/logica.h"
#include "../include/screen.h"
#include "../include/keyboard.h"

// --- BANCO DE PERGUNTAS (Cópia exata do seu JSON) ---

// Perguntas Normais (Rodada)
static const char* PERGUNTAS_NORMAIS[][4] = {
    {"Se P eh V e Q eh F, qual o valor de P ^ Q?", "Verdadeiro", "Falso", "1"}, // Correta: 2 (Falso) - Index 1
    {"Se P eh V e Q eh F, qual o valor de P v Q?", "Verdadeiro", "Falso", "0"}, // Correta: 1 (Verdadeiro) - Index 0
    {"Se P eh F e Q eh F, qual o valor de P v Q?", "Verdadeiro", "Falso", "1"},
    {"Se P eh F e Q eh V, qual o valor de P -> Q?", "Verdadeiro", "Falso", "0"},
    {"A operacao ~P representa:", "P eh verdadeiro", "A negacao de P", "1"},
    {"Se P <-> Q eh verdadeiro, entao:", "P e Q tem mesmo valor", "P e Q sao falsos", "0"},
    {"Qual eh o resultado de ~(Verdadeiro)?", "Verdadeiro", "Falso", "1"},
    {"Se P eh V e Q eh V, qual o valor de P <-> Q?", "Verdadeiro", "Falso", "0"},
    {"A operacao P ^ Verdadeiro resulta em:", "P", "Falso", "0"},
    {"A operacao P v Falso resulta em:", "P", "Verdadeiro", "0"},
    {"Se P eh falso, qual eh o valor de ~P?", "Verdadeiro", "Falso", "0"},
    {"Se P eh verdadeiro, valor de P -> Verdadeiro?", "Verdadeiro", "Falso", "0"},
    {"Se P eh verdadeiro, valor de P -> Falso?", "Verdadeiro", "Falso", "1"},
    {"Se P eh falso, valor de P -> Verdadeiro?", "Verdadeiro", "Falso", "0"},
    {"A expressao P ^ ~P eh sempre:", "Falsa", "Verdadeira", "0"}, // Ajustado ordem visual
    {"A expressao P v ~P eh sempre:", "Verdadeira", "Falsa", "0"},
    {"Se P e Q sao V, P v (~Q ^ P) eh:", "Verdadeiro", "Falso", "0"},
    {"Se P eh F e Q eh V, ~P ^ Q eh:", "Verdadeiro", "Falso", "0"},
    {"Qual operacao exige ambos verdadeiros?", "Ou (v)", "E (^)", "1"},
    {"Qual operacao so eh falsa se V -> F?", "Ou (v)", "Implica (->)", "1"} 
};

// Perguntas Difíceis (Tubarão)
static const char* PERGUNTAS_DIFICEIS[][4] = {
    {"Se P=V, Q=F, valor de (P ^ Q) v (~Q)?", "Verdadeiro", "Falso", "0"},
    {"Qual expressao equivalente a ~(P v Q)?", "~P ^ ~Q", "~P v ~Q", "0"},
    {"Tabela P -> Q eh falsa apenas quando:", "P=V e Q=F", "P=F e Q=V", "0"},
    {"Qual destas proposicoes eh tautologia?", "P v ~P", "P ^ ~P", "0"},
    {"Se P=F, Q=V, valor de (P -> Q) ^ (Q -> P)?", "Verdadeiro", "Falso", "1"},
    {"Expressao ~(P -> Q) eh equivalente a:", "P ^ ~Q", "~P ^ Q", "0"},
    {"Qual expressao equivalente a (P <-> Q)?", "(P^Q)v(~P^~Q)", "(P^~Q)", "0"}
};

// --- INTERFACE DE PERGUNTA (GUI) ---
int fazer_pergunta_gui(const char* p, const char* r1, const char* r2, int indice_correta) {
    // Define a posição Y para desenhar ABAIXO do tabuleiro
    // Tabuleiro tem altura ~17 (15 + bordas). HUD está no 18. Começamos no 20.
    int START_Y = MINY + ALTURA_JOGO + 6;

    // Limpa a área das perguntas (apaga perguntas anteriores)
    screenSetColor(WHITE, BLACK);
    for(int i=0; i<10; i++) {
        screenGotoxy(MINX, START_Y + i);
        printf("                                                                                "); // Limpa linha
    }

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MINX, START_Y);     printf("=== RODADA DE LOGICA ===");
    
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, START_Y + 2); printf("PERGUNTA: %s", p);
    
    screenGotoxy(MINX, START_Y + 4); printf("1) %s", r1);
    screenGotoxy(MINX, START_Y + 5); printf("2) %s", r2);
    
    screenSetColor(CYAN, BLACK);
    screenGotoxy(MINX, START_Y + 7); printf("Digite [1] ou [2] (ou Q para sair): ");
    
    char ch = ' ';
    while(ch != '1' && ch != '2' && ch != 'q' && ch != 'Q') {
        if(keyhit()) ch = readch();
    }
    
    if (ch == 'q' || ch == 'Q') return -1;
    
    // Converte char '1'/'2' para int 0/1 (indices do array)
    int resp_usuario = (ch == '1') ? 0 : 1;
    
    return (resp_usuario == indice_correta) ? 1 : 0;
}

// --- FUNÇÕES DE LÓGICA PRINCIPAL ---

void jogo_inicializar_tubaroes(Tabuleiro *tab) {
    jogo_resetar_tubaroes(tab);
}

void jogo_resetar_tubaroes(Tabuleiro *tab) {
    for(int i=0; i<tab->linhas; i++)
        for(int j=0; j<tab->colunas; j++)
            if(tab->matriz[i][j] == 'S') tab->matriz[i][j] = '.';

    // Cria 6 tubarões
    for(int k=0; k<6; k++) {
        int rL = rand() % (tab->linhas - 4) + 2;
        int rC = rand() % (tab->colunas - 4) + 2;
        if (rL > 5 || rC > 5) tab->matriz[rL][rC] = 'S';
    }
}

int jogo_fase_perguntas(Jogador *j) {
    int qtd_perguntas = 20; // Tamanho do array PERGUNTAS_NORMAIS
    
    for(int i=0; i<2; i++) {
        int idx = rand() % qtd_perguntas;
        
        // O ultimo elemento do array é a resposta correta em string ("0" ou "1")
        int correta = atoi(PERGUNTAS_NORMAIS[idx][3]);

        int res = fazer_pergunta_gui(
            PERGUNTAS_NORMAIS[idx][0], 
            PERGUNTAS_NORMAIS[idx][1], 
            PERGUNTAS_NORMAIS[idx][2], 
            correta
        );
        
        int START_Y = MINY + ALTURA_JOGO + 6;
        screenGotoxy(MINX, START_Y + 9);
        
        if (res == -1) return 0; // Sair

        if (res == 1) {
            j->pontuacao += PONTOS_NORMAL;
            screenSetColor(GREEN, BLACK);
            printf("ACERTOU! +%d pts. (Pressione ENTER)", PONTOS_NORMAL);
        } else {
            screenSetColor(RED, BLACK);
            printf("ERROU! (Pressione ENTER)");
        }
        
        while(!keyhit()); readch(); // Pause
        while(keyhit()) readch();   // Flush buffer
    }
    return 1;
}

int jogo_pergunta_tubarao(Jogador *j) {
    int qtd_perguntas = 7; // Tamanho do array PERGUNTAS_DIFICEIS
    int idx = rand() % qtd_perguntas;
    
    int correta = atoi(PERGUNTAS_DIFICEIS[idx][3]);

    int res = fazer_pergunta_gui(
        PERGUNTAS_DIFICEIS[idx][0], 
        PERGUNTAS_DIFICEIS[idx][1], 
        PERGUNTAS_DIFICEIS[idx][2], 
        correta
    );

    int START_Y = MINY + ALTURA_JOGO + 6;
    screenGotoxy(MINX, START_Y + 9);

    if (res == 1) {
        j->pontuacao += PONTOS_DIFICIL;
        screenSetColor(GREEN, BLACK);
        printf("ESCAPOU! +%d pts. (Pressione ENTER)", PONTOS_DIFICIL);
        return 1;
    } else {
        j->vidas--;
        screenSetColor(RED, BLACK);
        printf("ERROU! -1 VIDA. (Pressione ENTER)");
        return 0;
    }
}

void jogo_mover_tubaroes(Tabuleiro *tab, Jogador *j) {
    char **novaMatriz = (char**)malloc(tab->linhas * sizeof(char*));
    for(int i=0; i<tab->linhas; i++) {
        novaMatriz[i] = (char*)malloc(tab->colunas);
        memcpy(novaMatriz[i], tab->matriz[i], tab->colunas);
    }

    for(int y=0; y<tab->linhas; y++) {
        for(int x=0; x<tab->colunas; x++) {
            if(tab->matriz[y][x] == 'S') {
                novaMatriz[y][x] = '.'; 
                
                int novoY = y, novoX = x;
                
                if (abs(x - j->x) > abs(y - j->y)) {
                    if (x < j->x) novoX++; else if (x > j->x) novoX--;
                } else {
                    if (y < j->y) novoY++; else if (y > j->y) novoY--;
                }
                
                // Validação de movimento
                if (novoX >= 0 && novoX < tab->colunas && 
                    novoY >= 0 && novoY < tab->linhas && 
                    novaMatriz[novoY][novoX] == '.') {
                    novaMatriz[novoY][novoX] = 'S';
                } else {
                    novaMatriz[y][x] = 'S'; // Bloqueado
                }
            }
        }
    }
    
    for(int i=0; i<tab->linhas; i++) {
        memcpy(tab->matriz[i], novaMatriz[i], tab->colunas);
        free(novaMatriz[i]);
    }
    free(novaMatriz);
}

void desenhar_HUD(Jogador *j) {
    int Y_HUD = MINY + ALTURA_JOGO + 2; // Logo abaixo da borda inferior
    
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MINX, Y_HUD);
    for(int i=MINX; i<=MAXX; i++) printf("-"); 
    
    screenGotoxy(MINX + 2, Y_HUD + 1);
    printf(" %s PONTOS: %d  |  VIDAS: %d  |  [WASD] Mover | [Q] Sair ", EMOJI_JOGADOR, j->pontuacao, j->vidas);
}