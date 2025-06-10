#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // Para verificar existência do arquivo
#include "auxiliar.h"

// Função para verificar se o arquivo existe
int arquivo_existe(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Função para carregar os pontos do arquivo de texto
void carregarPontos(const char *arquivoPontos, Ponto **pontos, int *numPontos)
{
    if (!arquivo_existe(arquivoPontos))
    {
        fprintf(stderr, "Erro: O arquivo %s não foi encontrado.\n", arquivoPontos);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(arquivoPontos, "r");
    if (!file)
    {
        perror("Erro ao abrir arquivo de pontos");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", numPontos) != 1)
    {
        fprintf(stderr, "Erro ao ler o número de pontos no arquivo %s\n", arquivoPontos);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    *pontos = (Ponto *)malloc((*numPontos) * sizeof(Ponto));
    if (!*pontos)
    {
        perror("Erro ao alocar memória para os pontos");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *numPontos; i++)
    {
        if (fscanf(file, " %c %f %f %s %s",
                   &((*pontos)[i].id), &((*pontos)[i].x), &((*pontos)[i].y),
                   (*pontos)[i].rua1, (*pontos)[i].rua2) != 5)
        {
            fprintf(stderr, "Erro ao ler o ponto %d no arquivo %s\n", i, arquivoPontos);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

// Função para carregar os vizinhos do arquivo de texto
void carregarVizinhos(const char *arquivoVizinhos, Vizinho **vizinhos, int *numVizinhos)
{
    if (!arquivo_existe(arquivoVizinhos))
    {
        fprintf(stderr, "Erro: O arquivo %s nao foi encontrado.\n", arquivoVizinhos);
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen(arquivoVizinhos, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de vizinhos");
        exit(EXIT_FAILURE);
    }

    // Leia o número de vizinhos
    if (fscanf(file, "%d", numVizinhos) != 1) {
        fprintf(stderr, "Erro: Não foi possível ler o número de vizinhos\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    printf("Numero de vizinhos lido: %d\n", *numVizinhos);

    *vizinhos = (Vizinho *)malloc((*numVizinhos) * sizeof(Vizinho));
    if (!*vizinhos) {
        perror("Erro ao alocar memória para os vizinhos");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Nova linha
    int ch;
    while ((ch = fgetc(file)) != EOF && ch != '\n');

    for (int i = 0; i < *numVizinhos; i++) {
        char linha[256];
        
        // Leia toda a linha
        if (fgets(linha, sizeof(linha), file) == NULL) {
            fprintf(stderr, "Erro: Fim inesperado do arquivo ao ler vizinho %d\n", i);
            fclose(file);
            free(*vizinhos);
            exit(EXIT_FAILURE);
        }

        // Imprimir linha para depuração
        printf("Linha lida: %s", linha);

        // Analisar linha manualmente
        char *token = strtok(linha, " \n");
        if (!token) {
            fprintf(stderr, "Erro: Não foi possível ler origem do vizinho %d\n", i);
            fclose(file);
            free(*vizinhos);
            exit(EXIT_FAILURE);
        }
        (*vizinhos)[i].origem = token[0];

        token = strtok(NULL, " \n");
        if (!token) {
            fprintf(stderr, "Erro: Não foi possível ler destino do vizinho %d\n", i);
            fclose(file);
            free(*vizinhos);
            exit(EXIT_FAILURE);
        }
        (*vizinhos)[i].destino = token[0];

        token = strtok(NULL, "\n");
        if (!token) {
            fprintf(stderr, "Erro: Não foi possível ler rua do vizinho %d\n", i);
            fclose(file);
            free(*vizinhos);
            exit(EXIT_FAILURE);
        }
        strncpy((*vizinhos)[i].rua, token, sizeof((*vizinhos)[i].rua) - 1);
        (*vizinhos)[i].rua[sizeof((*vizinhos)[i].rua) - 1] = '\0';

        // Distância padrão
        (*vizinhos)[i].distancia = 10.0;
    }

    fclose(file);
}
// Função para executar o processamento completo
void executarProcessamento(const char *arquivoPontos, const char *arquivoVizinhos, const char *arquivoBinario)
{
    Ponto *pontos = NULL;
    Vizinho *vizinhos = NULL;
    int numPontos, numVizinhos;

    carregarPontos(arquivoPontos, &pontos, &numPontos);
    carregarVizinhos(arquivoVizinhos, &vizinhos, &numVizinhos);

    FILE *binFile = fopen(arquivoBinario, "wb");
    if (!binFile)
    {
        perror("Erro ao abrir arquivo binário");
        free(pontos);
        free(vizinhos);
        exit(EXIT_FAILURE);
    }

    if (fwrite(&numPontos, sizeof(int), 1, binFile) != 1)
    {
        perror("Erro ao escrever o número de pontos no arquivo binário");
        fclose(binFile);
        free(pontos);
        free(vizinhos);
        exit(EXIT_FAILURE);
    }
    if (fwrite(pontos, sizeof(Ponto), numPontos, binFile) != numPontos)
    {
        perror("Erro ao escrever os pontos no arquivo binário");
        fclose(binFile);
        free(pontos);
        free(vizinhos);
        exit(EXIT_FAILURE);
    }

    if (fwrite(&numVizinhos, sizeof(int), 1, binFile) != 1)
    {
        perror("Erro ao escrever o número de vizinhos no arquivo binário");
        fclose(binFile);
        free(pontos);
        free(vizinhos);
        exit(EXIT_FAILURE);
    }
    if (fwrite(vizinhos, sizeof(Vizinho), numVizinhos, binFile) != numVizinhos)
    {
        perror("Erro ao escrever os vizinhos no arquivo binário");
        fclose(binFile);
        free(pontos);
        free(vizinhos);
        exit(EXIT_FAILURE);
    }

    fclose(binFile);
    free(pontos);
    free(vizinhos);
}
