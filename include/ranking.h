#ifndef RANKING_H
#define RANKING_H

#define ARQUIVO_RANKING "../data/ranking.json"
#define MAX_RANKING 5

typedef struct {
    char nome[11];
    int pontos;
} RegistroRanking;

// Carrega o ranking do arquivo JSON
void carregar_ranking(RegistroRanking lista[], int *qtd);

// Salva a lista atual no arquivo JSON
void salvar_ranking(RegistroRanking lista[], int qtd);

// Adiciona um novo score, ordena e mantém apenas os top 5
void adicionar_pontuacao(const char* nome, int pontos);

// Exibe o ranking na tela usando a biblioteca screen.h
void desenhar_tela_ranking();

#endif