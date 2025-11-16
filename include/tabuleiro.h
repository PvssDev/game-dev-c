#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

typedef struct {
    int linhas;
    int colunas;
    char **matriz;
} Tabuleiro;

// Cria o tabuleiro gigante 80x30
Tabuleiro* criar_tabuleiro_gigante();

// Destrói o tabuleiro
void destruir_tabuleiro(Tabuleiro *tab);

// Desenha o tabuleiro gigante
void desenhar_tabuleiro(Tabuleiro *tab, int jogadorX, int jogadorY);

#endif
