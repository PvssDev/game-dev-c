#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

typedef struct {
    int linhas;
    int colunas;
    char **matriz; // matriz de caracteres para representar o mapa
} Tabuleiro;

// Cria o tabuleiro dinamicamente
Tabuleiro* criar_tabuleiro(int linhas, int colunas);

// Destrói o tabuleiro
void destruir_tabuleiro(Tabuleiro *tab);

// Desenha o tabuleiro na tela (inclui bordas e jogador)
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

#endif /* __TABULEIRO_H__ */

// por enquanto está só a estrutura do tabuleiro, sem tubarões
