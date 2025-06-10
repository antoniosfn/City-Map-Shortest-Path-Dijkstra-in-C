#ifndef AUXILIAR_H
#define AUXILIAR_H
#define MAX_RUA 100
#define DESCRICAO_MAX 100
#define RUA_MAX 100

// Estrutura para representar um ponto no grafo

typedef struct {
    char id;
    float x, y;
    char rua1[MAX_RUA];
    char rua2[MAX_RUA];
} Ponto;

// Estrutura para representar um vizinho no grafo
typedef struct
{
    char origem;
    char destino;
    double distancia;
    char rua[RUA_MAX];
} Vizinho;

// Estrutura para representar o grafo
typedef struct
{
    int numPontos;
    Ponto *pontos;
    int numVizinhos;
    Vizinho *vizinhos;
} Grafo;

// Função para processar os dados dos arquivos e criar o arquivo binário
void executarProcessamento(const char *arquivoPontos, const char *arquivoVizinhos, const char *arquivoBinario);

#endif
