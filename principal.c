#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "auxiliar.h"

#define MAX_NODES 2000
#define INFINITO 100000.0

// Função para encontrar a aresta entre dois pontos no grafo
Vizinho *encontrarAresta(Grafo *grafo, int u, int v)
{
    for (int i = 0; i < grafo->numVizinhos; i++)
    {
        if (grafo->vizinhos[i].origem == grafo->pontos[u].id &&
            grafo->vizinhos[i].destino == grafo->pontos[v].id)
        {
            return &grafo->vizinhos[i];
        }
    }
    return NULL;
}

// Função com o Algoritmo de Dijkstra para encontrar o menor caminho
void encontrarMenorCaminho(Grafo *grafo, int origem, int destino)
{
    double dist[MAX_NODES];
    int anterior[MAX_NODES];
    int visitado[MAX_NODES] = {0};

    for (int i = 0; i < grafo->numPontos; i++)
    {
        dist[i] = INFINITO;
        anterior[i] = -1;
    }
    dist[origem] = 0;

    for (int i = 0; i < grafo->numPontos; i++)
    {
        int u = -1;
        for (int j = 0; j < grafo->numPontos; j++)
        {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u]))
            {
                u = j;
            }
        }

        if (u == -1 || dist[u] == INFINITO)
            break;

        visitado[u] = 1;

        for (int j = 0; j < grafo->numVizinhos; j++)
        {
            if (grafo->vizinhos[j].origem == grafo->pontos[u].id)
            {
                int v;
                for (v = 0; v < grafo->numPontos; v++)
                {
                    if (grafo->pontos[v].id == grafo->vizinhos[j].destino)
                        break;
                }

                double alt = dist[u] + grafo->vizinhos[j].distancia;
                if (alt < dist[v])
                {
                    dist[v] = alt;
                    anterior[v] = u; // Atualiza o vetor anterior
                }
            }
        }
    }

    if (dist[destino] == INFINITO)
    {
        printf("Não existe caminho entre os pontos escolhidos.\n");
        return;
    }

    printf("Para realizar o percurso:\n");

    int caminho[MAX_NODES];
    int tamanhoCaminho = 0;
    int atual = destino;

    while (atual != -1)
    {
        caminho[tamanhoCaminho++] = atual;
        atual = anterior[atual];
    }

    for (int i = tamanhoCaminho - 1; i > 0; i--)
    {
        int u = caminho[i];
        int v = caminho[i - 1];
        Vizinho *aresta = encontrarAresta(grafo, u, v);

        if (aresta)
        {
            printf("(%d) Siga pela %s ate %c\n", tamanhoCaminho - i, aresta->rua, grafo->pontos[v].id);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Uso: %s <arquivo_pontos> <arquivo_vizinhos> <arquivo_binario>\n", argv[0]);
        return 1;
    }

    executarProcessamento(argv[1], argv[2], argv[3]);

    FILE *arquivoBinario = fopen(argv[3], "rb");
    if (!arquivoBinario)
    {
        perror("Erro ao abrir arquivo binario");
        return 1;
    }

    Grafo grafo;
    if (fread(&grafo.numPontos, sizeof(int), 1, arquivoBinario) != 1)
    {
        fprintf(stderr, "Erro ao ler o numero de pontos\n");
        fclose(arquivoBinario);
        return 1;
    }

    grafo.pontos = (Ponto *)malloc(grafo.numPontos * sizeof(Ponto));
    if (!grafo.pontos || fread(grafo.pontos, sizeof(Ponto), grafo.numPontos, arquivoBinario) != grafo.numPontos)
    {
        fprintf(stderr, "Erro ao ler os pontos\n");
        fclose(arquivoBinario);
        free(grafo.pontos);
        return 1;
    }

    if (fread(&grafo.numVizinhos, sizeof(int), 1, arquivoBinario) != 1)
    {
        fprintf(stderr, "Erro ao ler o numero de vizinhos\n");
        fclose(arquivoBinario);
        free(grafo.pontos);
        return 1;
    }

    grafo.vizinhos = (Vizinho *)malloc(grafo.numVizinhos * sizeof(Vizinho));
    if (!grafo.vizinhos || fread(grafo.vizinhos, sizeof(Vizinho), grafo.numVizinhos, arquivoBinario) != grafo.numVizinhos)
    {
        fprintf(stderr, "Erro ao ler os vizinhos\n");
        fclose(arquivoBinario);
        free(grafo.pontos);
        free(grafo.vizinhos);
        return 1;
    }

    fclose(arquivoBinario);

    char origem_id, destino_id;
    printf("Digite o ponto de origem: ");
    if (scanf(" %c", &origem_id) != 1)
    {
        fprintf(stderr, "Erro ao ler o ponto de origem\n");
        return 1;
    }

    printf("Digite o ponto de destino: ");
    if (scanf(" %c", &destino_id) != 1)
    {
        fprintf(stderr, "Erro ao ler o ponto de destino\n");
        return 1;
    }

    int origem = -1, destino = -1;
    for (int i = 0; i < grafo.numPontos; i++)
    {
        if (grafo.pontos[i].id == origem_id)
            origem = i;
        if (grafo.pontos[i].id == destino_id)
            destino = i;
    }

    if (origem == -1 || destino == -1)
    {
        printf("Ponto(s) invalido(s) de origem ou destino.\n");
    }
    else
    {
        encontrarMenorCaminho(&grafo, origem, destino);
    }

    free(grafo.pontos);
    free(grafo.vizinhos);

    return 0;
}
